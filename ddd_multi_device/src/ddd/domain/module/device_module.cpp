#include "ddd/domain/module/device_module.hpp"

#include <utility>

namespace ddd::domain::module {

DeviceModule::DeviceModule(core::Id id, std::string name, ModuleType type)
    : id_(id), name_(std::move(name)), type_(type) {}

// INACTIVE -> PREHEATING (任何模块都能上电).
core::Result<bool> DeviceModule::powerOn() {
    if (state_ != ModuleState::INACTIVE)
        return core::Result<bool>::failure("非 INACTIVE 态不能上电");
    state_ = ModuleState::PREHEATING;
    return core::Result<bool>::success(true);
}

// PREHEATING -> READY. Guard: 复合节点需全部子模块 READY; 执行型需传感 ok.
core::Result<bool> DeviceModule::preheatDone() {
    if (state_ != ModuleState::PREHEATING)
        return core::Result<bool>::failure("非 PREHEATING 态不能判定就绪");
    if (!canReachReady())
        return core::Result<bool>::failure("Guard 不满足: 子模块/传感尚未就绪");
    state_ = ModuleState::READY;
    return core::Result<bool>::success(true);
}

// READY -> WORKING. 仅 READY 的可控模块可 start; 绝不允许带 FAULT 进入.
core::Result<bool> DeviceModule::startWork() {
    if (type_ == ModuleType::SENSOR)
        return core::Result<bool>::failure("传感型模块不接受控制指令");
    if (state_ != ModuleState::READY)
        return core::Result<bool>::failure("仅 READY 态可开工");
    if (!sensorOk())
        return core::Result<bool>::failure("传感条件不满足, 禁止启动");
    state_ = ModuleState::WORKING;
    return core::Result<bool>::success(true);
}

// WORKING -> COMPLETED.
core::Result<bool> DeviceModule::finishWork() {
    if (state_ != ModuleState::WORKING)
        return core::Result<bool>::failure("仅 WORKING 态可完成");
    state_ = ModuleState::COMPLETED;
    return core::Result<bool>::success(true);
}

// 任意态 -> FAULT; 不可带着 FAULT 再开工(由 startWork 的 Guard 保证).
core::Result<bool> DeviceModule::setFault() {
    if (state_ == ModuleState::FAULT)
        return core::Result<bool>::failure("已在 FAULT 态");
    state_ = ModuleState::FAULT;
    return core::Result<bool>::success(true);
}

// FAULT -> INACTIVE (现场复位).
core::Result<bool> DeviceModule::reset() {
    if (state_ != ModuleState::FAULT)
        return core::Result<bool>::failure("仅 FAULT 态需要/允许复位");
    state_ = ModuleState::INACTIVE;
    return core::Result<bool>::success(true);
}

void DeviceModule::refreshSensor(SensorState input) {
    sensor_ = input;  // 传感状态只由外部输入驱动
}

bool DeviceModule::canReachReady() const {
    // 复合模块: READY 当且仅当 所有子模块就绪(或与其对应的传感条件就绪, 见需求 §4).
    for (auto* c : children_) {
        if (!c) return false;
        bool childReady =
            (c->type_ == ModuleType::SENSOR) ? (c->sensor() == SensorState::NORMAL)
                                             : (c->state_ == ModuleState::READY);
        if (!childReady) return false;
    }
    return true;
}

bool DeviceModule::sensorOk() const {
    // 传感型模块自身作为 Guard 时, 要求处于 NORMAL.
    if (type_ == ModuleType::SENSOR)
        return sensor_ == SensorState::NORMAL;
    // 执行型: 若挂有传感型子模块, 需要它传感正常.
    for (auto* c : children_) {
        if (c && c->type_ == ModuleType::SENSOR && c->sensor() != SensorState::NORMAL)
            return false;
    }
    return true;
}

}  // namespace ddd::domain::module