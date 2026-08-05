// Composition root for the multi-layer X machine demo.
//
// Demonstrates the layered DDD model:
//   * multiple layers (上位机 -> 工位 -> 设备/传感),
//   * downward RPC  (IRpc 下发控制命令: advance/start/finish),
//   * upward Pub/Sub (IUpstream 逐级上报状态/故障),
//   * rich readiness plan (上电/预热/清洗/自检/到位 -> 就绪),
//   * fault impact policy + 逐级上报 (判断是否影响生产并向上传导).

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ddd/application/plant_controller.hpp"
#include "ddd/domain/core/id.hpp"
#include "ddd/domain/module/device_module.hpp"
#include "ddd/domain/process/model.hpp"
#include "ddd/infrastructure/messaging/event_bus.hpp"
#include "ddd/infrastructure/runtime/store.hpp"

using namespace ddd;
using namespace ddd::domain;
using namespace ddd::application;
using infrastructure::messaging::EventBus;

using module::DeviceModule;
using module::ModuleState;
using module::ModuleType;
using module::SensorState;
using process::FaultLevel;
using process::Step;

// Wire a domain event bus: subscribe listeners (SCADA 上位机/日志/告警中心),
// then inject the bus into every deployed node so Alarm/StateChanged/Ready
// are published as domain events.
static void attachBus(EventBus& bus,
                      infrastructure::runtime::NodeStore& store) {
    bus.subscribe([&](const domain::events::DomainEvent& e) {
        std::cout << "      [📣 事件] " << domain::events::eventKindLabel(e.kind)
                  << " id=" << e.moduleId.toString()
                  << " " << e.from << "->" << e.to;
        if (!e.message.empty()) std::cout << " (" << e.message << ")";
        std::cout << "\n";
    });
    for (auto& [id, n] : store.all()) n->setEventSink(&bus);
}

static void scenarioNormal() {
    std::cout << "\n########## 场景A：多层级产线正常上电 -> 工作 ##########\n";
    infrastructure::runtime::NodeStore store;
    infrastructure::runtime::RpcRouter router(&store);
    PlantController ctrl(&store);
    EventBus bus;
    (void)bus;

    process::HierNode* line = nullptr;
    // --- build all nodes (owned by store) ---
    auto add = [&](core::Id id, std::string name, int layer, std::vector<Step> plan,
                   bool critical) {
        auto n = std::make_unique<process::HierNode>(id, name, layer);
        n->setPlan(std::move(plan));
        n->setCriticalToJob(critical);
        n->setRpc(&router);
        store.add(std::move(n));
    };

    add({1}, "产线", 0, {Step::POWER}, true);
    add({11}, "预热工位", 1, {Step::POWER}, true);
    add({12}, "加工工位", 1, {Step::POWER, Step::PREHEAT}, true);
    add({13}, "品检工位", 1, {Step::POWER}, true);
    add({111}, "加热器Heater", 2, {Step::POWER, Step::PREHEAT, Step::SELFCHECK, Step::HOME}, true);
    add({112}, "温度传感(pre-hot)", 2, {Step::POWER}, false);   // 关键性:false
    add({121}, "精密定位DP-200", 2, {Step::POWER, Step::HOME, Step::SELFCHECK}, true);
    add({131}, "质检相机QCcam", 2, {Step::POWER, Step::SELFCHECK, Step::HOME}, false);

    // parent/child wiring + upstream chain to 上位机.
    line = store.at({1});
    auto st1 = store.at({11});
    auto st2 = store.at({12});
    auto st3 = store.at({13});

    line->setUpstream(&ctrl);
    line->addChild(st1->id());
    line->addChild(st2->id());
    line->addChild(st3->id());

    st1->setUpstream(line);
    st1->addChild(store.at({111})->id());
    st1->addChild(store.at({112})->id());

    st2->setUpstream(line);
    st2->addChild(store.at({121})->id());

    st3->setUpstream(line);
    st3->addChild(store.at({131})->id());

    store.at({111})->setUpstream(st1);
    store.at({112})->setUpstream(st1);
    store.at({121})->setUpstream(st2);
    store.at({131})->setUpstream(st3);

    attachBus(bus, store);

    for (auto& [id, n] : store.all()) {
        n->setTrace([](const domain::port::ProcessReport& r) {
            std::cout << "   [↑升报] " << r.name << "[L" << r.layer << "] -> "
                      << "step=" << stepLabel(r.step)
                      << " ready=" << (r.ready ? "Y" : "N")
                      << " affects=" << (r.affectsProduction ? "Y" : "N")
                      << "\n";
        });
    }

    std::cout << "--- 上位机下发: 预热并到达工作位置 (逐层 RPC 下发) ---\n";
    ctrl.runToReady({1});

    std::cout << "--- 设备就绪状态(报表) ---\n";
    ctrl.dumpAll();

    std::cout << "--- 开始生产 ---\n";
    ctrl.startWork({1});
    ctrl.dumpAll();
}

static void scenarioFault() {
    std::cout << "\n========== 场景：运行中某设备故障 -> 判断影响并逐级上报 ==========\n";
    infrastructure::runtime::NodeStore store;
    infrastructure::runtime::RpcRouter router(&store);
    PlantController ctrl(&store);
    EventBus bus;
    (void)bus;

    auto add = [&](core::Id id, std::string name, int layer, std::vector<Step> plan,
                   bool critical) {
        auto n = std::make_unique<process::HierNode>(id, name, layer);
        n->setPlan(std::move(plan));
        n->setCriticalToJob(critical);
        n->setRpc(&router);
        store.add(std::move(n));
    };
    add({1}, "产线", 0, {Step::POWER}, true);
    add({11}, "预热工位", 1, {Step::POWER}, true);
    add({12}, "加工工位", 1, {Step::POWER}, true);
    add({111}, "加热器Heater", 2, {Step::POWER, Step::PREHEAT, Step::SELFCHECK, Step::HOME}, true);
    add({112}, "温度感应器(非关键告警)", 2, {Step::POWER}, false);
    add({121}, "精密机DP-200", 2, {Step::POWER, Step::HOME, Step::SELFCHECK}, true);

    auto line = store.at({1});
    auto st1 = store.at({11});
    auto st2 = store.at({12});
    line->setUpstream(&ctrl);
    line->addChild({11});
    line->addChild({12});
    st1->setUpstream(line);
    st1->addChild({111});
    st1->addChild({112});
    st2->setUpstream(line);
    st2->addChild({121});
    store.at({111})->setUpstream(st1);
    store.at({112})->setUpstream(st1);
    store.at({121})->setUpstream(st2);

    attachBus(bus, store);

    for (auto& [id, n] : store.all()) {
        n->setTrace([](const domain::port::ProcessReport& r) {
            std::cout << "   [↑升报] " << r.name << "[L" << r.layer << "] -> "
                      << "fault=" << faultLabel(r.fault)
                      << " affects=" << (r.affectsProduction ? "Y" : "N")
                      << " (" << r.reason << ")\n";
        });
    }

    ctrl.runToReady({1});
    std::cout << "稳定生产...\n";

    std::cout << "\n(1)【非关键设备告警】温度传感器报警(ALARM)，判断不影响生产：\n";
    ctrl.fail({112}, FaultLevel::ALARM);
    std::cout << "（传感非关键，站已运行，产线是否受影响应在上报中体现）\n";

    std::cout << "\n(1b) 清除该告警后继续\n";
    ctrl.clearFault({112});

    std::cout << "\n(2)【关键硬故障】DP-200 定位失效(BLOCKING)→判断阻断并逐级上报至产线：\n";
    ctrl.fail({121}, FaultLevel::BLOCKING);
    std::cout << "\n结果报表：\n";
    ctrl.dumpAll();

    std::cout << "\n(3) 现场复位后从上电重来：\n";
    ctrl.clearFault({121});
    ctrl.runToReady({1});
    ctrl.dumpAll();
}

static void scenarioSensorMapping() {
    std::cout << "\n========== 场景：传感映射(只读) 守护执行型模块的开工 Guard ==========\n";

    // 组合模块: 加工台(OPERATOR) 挂一个 传感型温度子模块.
    DeviceModule workStation({501}, "加工台DP-500", ModuleType::OPERATOR);
    DeviceModule tempSensor({502}, "温度传感", ModuleType::SENSOR);
    workStation.addChild(&tempSensor);

    auto print = [&](const char* action, const core::Result<bool>& r) {
        std::cout << "  [" << action << "] "
                  << (r.isOk() ? "成功" : "被拒: " + r.error())
                  << "  -> 加工台=" << moduleStateLabel(workStation.state())
                  << ", 传感=" << sensorStateLabel(tempSensor.sensor()) << "\n";
    };

    print("上电", workStation.powerOn());
    print("预热完成", workStation.preheatDone());  // 传感 NORMAL, 允许就绪
    print("尝试开工(传感正常)", workStation.startWork());
    print("完成", workStation.finishWork());

    std::cout << "\n--- 模拟故障复位后重来, 但传感失联(OUT_OF_RANGE) ---\n";
    print("置故障(硬故障)", workStation.setFault());
    print("现场复位", workStation.reset());
    std::cout << "  非法迁移演示: 复位后未上电就 preheatDone\n";
    DeviceModule illegal({999}, "非法模块", ModuleType::OPERATOR);
    print("(非法)直接判定就绪", illegal.preheatDone());

    tempSensor.refreshSensor(SensorState::OUT_OF_RANGE);  // 外部输入驱动, 传感型不可被指令写
    print("上电", workStation.powerOn());
    print("(Guard阻断) 传感失联, preheat 不满足", workStation.preheatDone());
    tempSensor.refreshSensor(SensorState::NORMAL);  // 传感器恢复
    print("预热完成(传感恢复)", workStation.preheatDone());
    print("开工", workStation.startWork());
}

int main() {
    scenarioSensorMapping();
    scenarioNormal();
    scenarioFault();
    return 0;
}