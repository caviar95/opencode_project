#pragma once
#include <string>

namespace ddd::domain::process {

// Rich preparation / runtime step. A node reaches READY only after walking
// through its plan (a subset of the steps below, in order).
enum class Step {
    UNKNOWN = 0,
    POWER,     // 上电
    PREHEAT,   // 设备预热
    CLEAN,     // 设备清洗
    SELFCHECK, // 设备故障检测 / 自检
    HOME,      // 运行到工作位置
    READY,     // 就绪（可开始工作）
    WORK,      // 工作中
    DONE       // 完成
};

// Fault severity. Decides whether production is affected, then escalates.
enum class FaultLevel { NONE = 0, ALARM, BLOCKING };

inline const char* stepLabel(Step s) {
    switch (s) {
        case Step::UNKNOWN: return "UNKNOWN";
        case Step::POWER: return "POWER(上电)";
        case Step::PREHEAT: return "PREHEAT(预热)";
        case Step::CLEAN: return "CLEAN(清洗)";
        case Step::SELFCHECK: return "SELFCHECK(故障检测)";
        case Step::HOME: return "HOME(到位)";
        case Step::READY: return "READY(就绪)";
        case Step::WORK: return "WORKING(工作)";
        case Step::DONE: return "DONE(完成)";
    }
    return "?";
}

inline const char* faultLabel(FaultLevel f) {
    switch (f) {
        case FaultLevel::NONE: return "NONE";
        case FaultLevel::ALARM: return "ALARM(告警/不影响生产)";
        case FaultLevel::BLOCKING: return "BLOCKING(阻断生产)";
    }
    return "?";
}

inline FaultLevel worstOf(FaultLevel a, FaultLevel b) {
    return a > b ? a : b;
}

inline Step stepOf(int index) {
    return static_cast<Step>(index);
}

}  // namespace ddd::domain::process