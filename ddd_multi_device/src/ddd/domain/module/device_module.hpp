#pragma once
#include <string>
#include <vector>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/core/result.hpp"

namespace ddd::domain::module {

// Matches docs/01-requirement §2/§5.2/§5.1.
enum class ModuleType { OPERATOR, SENSOR };

inline const char* moduleTypeLabel(ModuleType t) {
    return t == ModuleType::OPERATOR ? "OPERATOR(执行型)" : "SENSOR(传感型)";
}

enum class ModuleState { INACTIVE = 0, PREHEATING, READY, WORKING, COMPLETED, FAULT };

inline const char* moduleStateLabel(ModuleState s) {
    switch (s) {
        case ModuleState::INACTIVE: return "INACTIVE";
        case ModuleState::PREHEATING: return "PREHEATING(预热)";
        case ModuleState::READY: return "READY(就绪)";
        case ModuleState::WORKING: return "WORKING(工作)";
        case ModuleState::COMPLETED: return "COMPLETED(完成)";
        case ModuleState::FAULT: return "FAULT(故障)";
    }
    return "?";
}

// Sensor-mapped state (只读反映, 由外部传感器输入驱动).
enum class SensorState { NORMAL, TRIGGERED, OUT_OF_RANGE };

inline const char* sensorStateLabel(SensorState s) {
    switch (s) {
        case SensorState::NORMAL: return "NORMAL(正常)";
        case SensorState::TRIGGERED: return "TRIGGERED(触发)";
        case SensorState::OUT_OF_RANGE: return "OUT_OF_RANGE(失联/超范围)";
    }
    return "?";
}

// ---- Aggregate root: 设备模块 状态机 (docs §5.1 / §5.2) ----
// Every transition drags through a migration-allowed set + a Guard; an
// illegal move returns Result::failure instead of mutating state.
class DeviceModule {
   public:
    DeviceModule(core::Id id, std::string name, ModuleType type);

    // ---- 上层驱动命令(状态转换行为) ----
    core::Result<bool> powerOn();      // INACTIVE -> PREHEATING
    core::Result<bool> preheatDone();  // PREHEATING -> READY (Guard: 子模块/传感就绪)
    core::Result<bool> startWork();    // READY -> WORKING (仅 READY 可控)
    core::Result<bool> finishWork();   // WORKING -> COMPLETED
    core::Result<bool> setFault();     // 任意态 -> FAULT
    core::Result<bool> reset();        // FAULT -> INACTIVE (现场复位)

    // ---- 传感映射: 仅 SENSOR 模块可调用, 外部输入驱动 (不可由指令直接写) ----
    void refreshSensor(SensorState input);  // NORMAL -> TRIGGERED / OUT_OF_RANGE

    // ---- 组合(Composite): 子模块就绪的 Guard ----
    void addChild(DeviceModule* child) { children_.push_back(child); }

    // ---- queries ----
    core::Id id() const { return id_; }
    const std::string& name() const { return name_; }
    ModuleType type() const { return type_; }
    ModuleState state() const { return state_; }
    SensorState sensor() const { return sensor_; }
    bool inFault() const { return state_ == ModuleState::FAULT; }

private:
    bool canReachReady() const;  // Guard: 复合节点=全部子模块 READY; OPERATOR 依赖传感就绪
    bool sensorOk() const;       // OPERATOR 启动的传感 Guard

    core::Id id_;
    std::string name_;
    ModuleType type_;
    ModuleState state_{ModuleState::INACTIVE};
    SensorState sensor_{SensorState::NORMAL};
    std::vector<DeviceModule*> children_;
};

}  // namespace ddd::domain::module