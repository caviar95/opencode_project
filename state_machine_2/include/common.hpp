#pragma once

#include <string>
#include <ostream>
#include <functional>
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <iostream>
#include <optional>
#include <set>

enum class DeviceState {
    Off,
    Starting,
    Standby,
    Active,
    Suspended,
    Error,
    EmergencyStop,
    ShuttingDown,
    On,
    PowerOnState
};

inline std::string to_string(DeviceState s) {
    switch (s) {
        case DeviceState::Off: return "Off";
        case DeviceState::Starting: return "Starting";
        case DeviceState::Standby: return "Standby";
        case DeviceState::Active: return "Active";
        case DeviceState::Suspended: return "Suspended";
        case DeviceState::Error: return "Error";
        case DeviceState::EmergencyStop: return "EmergencyStop";
        case DeviceState::ShuttingDown: return "ShuttingDown";
        case DeviceState::On: return "On";
        case DeviceState::PowerOnState: return "PowerOnState";
    }
    return "Unknown";
}

inline std::ostream& operator<<(std::ostream& os, DeviceState s) {
    os << to_string(s);
    return os;
}

enum class DeviceEvent {
    PowerOn,
    PowerOff,
    StartComplete,
    EnterStandby,
    ExitStandby,
    Suspend,
    Resume,
    ErrorOccurred,
    ErrorCleared,
    EmergencyStopTriggered,
    Recover
};

inline std::string to_string(DeviceEvent e) {
    switch (e) {
        case DeviceEvent::PowerOn: return "PowerOn";
        case DeviceEvent::PowerOff: return "PowerOff";
        case DeviceEvent::StartComplete: return "StartComplete";
        case DeviceEvent::EnterStandby: return "EnterStandby";
        case DeviceEvent::ExitStandby: return "ExitStandby";
        case DeviceEvent::Suspend: return "Suspend";
        case DeviceEvent::Resume: return "Resume";
        case DeviceEvent::ErrorOccurred: return "ErrorOccurred";
        case DeviceEvent::ErrorCleared: return "ErrorCleared";
        case DeviceEvent::EmergencyStopTriggered: return "EmergencyStopTriggered";
        case DeviceEvent::Recover: return "Recover";
    }
    return "Unknown";
}

inline std::ostream& operator<<(std::ostream& os, DeviceEvent e) {
    os << to_string(e);
    return os;
}
