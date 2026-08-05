#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <hfsm/core/history.hpp>
#include <hfsm/ext/logger.hpp>
#include <hfsm/hfsm.hpp>
#include <string>
#include <thread>
#include <vector>

using namespace hfsm;

// ============================================================
// Example 5: Industrial Device Lifecycle Manager
//
// Demonstrates:
// - Deep state hierarchy with history
// - Power management with sub-states
// - Fault handling and recovery
// - Firmware update with progress tracking
// ============================================================

// ---- Events ----

struct PowerOn
{
};
struct PowerOff
{
};
struct Standby
{
};
struct WakeUp
{
};
struct Heartbeat
{
    uint32_t timestamp;
};
struct TemperatureReading
{
    double temp_celsius;
};
struct OverTemperature
{
    double temp;
};
struct NormalTemperature
{
};

struct FaultDetected
{
    int fault_code;
    char description[128];
};
struct FaultCleared
{
};
struct RecoveryInitiated
{
};
struct RecoveryComplete
{
};

struct StartFirmwareUpdate
{
    char version[32];
    size_t size_bytes;
};
struct FWDownloadProgress
{
    int percent;
};
struct FWDownloadComplete
{
};
struct FWVerifyFailure
{
    char detail[128];
};
struct FWUpdateComplete
{
};
struct FWUpdateFailed
{
    int error_code;
};

// ---- Device States (3-level hierarchy) ----
//
// Device (top)
//   |-- PoweredOff
//   |-- PoweredOn (composite)
//         |-- Active
//         |   |-- Idle
//         |   |-- Running
//         |   |-- Busy
//         |-- Standby
//         |-- Fault (composite)
//         |   |-- MinorFault
//         |   |-- MajorFault
//         |   |-- Recovering
//         |-- FirmwareUpdate (composite)
//             |-- FWDownloading
//             |-- FWVerifying
//             |-- FWInstalling

enum class DeviceState : StateId {
    PoweredOff,
    PoweredOn,
    // Sub-states of PoweredOn
    Active,
    Standby,
    Fault,
    FirmwareUpdate,
    // Sub-states of Active
    ActiveIdle,
    Running,
    Busy,
    // Sub-states of Fault
    MinorFault,
    MajorFault,
    Recovering,
    // Sub-states of FirmwareUpdate
    FWDownloading,
    FWVerifying,
    FWInstalling,
};

const char* device_state_name(DeviceState s)
{
    switch (s) {
    case DeviceState::PoweredOff:
        return "PoweredOff";
    case DeviceState::PoweredOn:
        return "PoweredOn";
    case DeviceState::Active:
        return "Active";
    case DeviceState::Standby:
        return "Standby";
    case DeviceState::Fault:
        return "Fault";
    case DeviceState::FirmwareUpdate:
        return "FirmwareUpdate";
    case DeviceState::ActiveIdle:
        return "Active/Idle";
    case DeviceState::Running:
        return "Active/Running";
    case DeviceState::Busy:
        return "Active/Busy";
    case DeviceState::MinorFault:
        return "Fault/Minor";
    case DeviceState::MajorFault:
        return "Fault/Major";
    case DeviceState::Recovering:
        return "Fault/Recovering";
    case DeviceState::FWDownloading:
        return "FW/Downloading";
    case DeviceState::FWVerifying:
        return "FW/Verifying";
    case DeviceState::FWInstalling:
        return "FW/Installing";
    }
    return "UNKNOWN";
}

// Device metrics
struct DeviceMetrics
{
    uint64_t uptime_seconds = 0;
    int fault_count = 0;
    int recovery_count = 0;
    int fw_update_count = 0;
    double max_temperature = 0.0;
    double current_temperature = 25.0;
    uint32_t last_heartbeat = 0;
    int total_transitions = 0;
};

class DeviceManager
{
public:
    StateMachineEngine engine;
    HistoryManager history;
    DeviceMetrics metrics;
    ModuleLogger log{"DeviceManager"};

    DeviceManager() : log("DeviceManager")
    {
        // Register all states
        for (int i = 0; i <= static_cast<int>(DeviceState::FWInstalling); i++) {
            auto s = static_cast<DeviceState>(i);
            engine.register_state(static_cast<StateId>(s));
            engine.set_state_name(static_cast<StateId>(s),
                                  device_state_name(s));
        }

        engine.set_initial(static_cast<StateId>(DeviceState::PoweredOff));

        // Configure history for composite states
        history.configure(static_cast<StateId>(DeviceState::Active),
                          HistoryMode::Deep);
        history.configure(static_cast<StateId>(DeviceState::Fault),
                          HistoryMode::Shallow);

        // ---- Entry/Exit Callbacks ----
        setup_callbacks();

        // ---- Transitions ----
        setup_transitions();

        // Set logger
        engine.set_logger(
            [this](const std::string& msg) { log.debug("%s", msg.c_str()); });
    }

    void run()
    {
        std::printf("\n=== Device Lifecycle Simulation ===\n\n");

        log.info("=== Scenario 1: Normal Operation ===");
        normal_operation();

        log.info("\n=== Scenario 2: Fault with Auto-Recovery ===");
        fault_recovery();

        log.info("\n=== Scenario 3: Firmware Update ===");
        firmware_update();

        log.info("\n=== Scenario 4: Power Cycle with Deep History ===");
        power_cycle_with_history();

        print_metrics();
    }

private:
    auto sid(DeviceState s)
    {
        return static_cast<StateId>(s);
    }

    void add_rule(DeviceState src,
                  const std::type_index& evt,
                  DeviceState dst,
                  std::function<bool(const EventEnvelope&)> guard = {},
                  std::function<void(const EventEnvelope&)> action = {})
    {
        engine.add_rule({sid(src), evt, sid(dst), false, false,
                         std::move(guard), std::move(action)});
    }

    void setup_callbacks()
    {
        // Level 1: Powered states
        engine.on_entry(sid(DeviceState::PoweredOff), [this](const auto&) {
            log.info("Device powered OFF");
        });

        engine.on_entry(sid(DeviceState::PoweredOn), [this](const auto&) {
            log.info("Device powered ON");
            history.record(sid(DeviceState::Active),
                           sid(DeviceState::ActiveIdle));
        });

        // Level 2: Active sub-states
        engine.on_entry(sid(DeviceState::ActiveIdle), [this](const auto&) {
            log.info("  [Active] Idle - waiting for commands");
        });
        engine.on_entry(sid(DeviceState::Running), [this](const auto&) {
            log.info("  [Active] Running - processing");
        });
        engine.on_entry(sid(DeviceState::Busy), [this](const auto&) {
            log.info("  [Active] Busy - high load");
        });
        engine.on_exit(sid(DeviceState::Busy), [this](const auto&) {
            log.info("  [Active] Busy completed");
        });

        // Level 2: Standby
        engine.on_entry(sid(DeviceState::Standby), [this](const auto&) {
            log.info("  [Standby] Low power mode");
            history.record(sid(DeviceState::Active), engine.current_state());
        });

        // Level 2: Fault
        engine.on_entry(sid(DeviceState::MinorFault), [this](const auto&) {
            metrics.fault_count++;
            log.warn("  [Fault] Minor fault detected (#%d)",
                     metrics.fault_count);
        });
        engine.on_entry(sid(DeviceState::MajorFault), [this](const auto&) {
            metrics.fault_count++;
            log.error("  [Fault] MAJOR fault detected (#%d)",
                      metrics.fault_count);
        });
        engine.on_entry(sid(DeviceState::Recovering), [this](const auto&) {
            metrics.recovery_count++;
            log.info("  [Fault] Recovery in progress (attempt #%d)",
                     metrics.recovery_count);
        });

        // Level 2/3: Firmware Update
        engine.on_entry(sid(DeviceState::FWDownloading), [this](const auto&) {
            log.info("  [FW] Downloading firmware...");
        });
        engine.on_entry(sid(DeviceState::FWVerifying), [this](const auto&) {
            log.info("  [FW] Verifying firmware integrity...");
        });
        engine.on_entry(sid(DeviceState::FWInstalling), [this](const auto&) {
            log.info("  [FW] Installing firmware...");
        });
    }

    void setup_transitions()
    {
        // ---- Level 0: Power ----
        add_rule(DeviceState::PoweredOff, typeid(PowerOn),
                 DeviceState::PoweredOn);
        add_rule(DeviceState::PoweredOn, typeid(PowerOff),
                 DeviceState::PoweredOff);

        // ---- Level 1: PoweredOn sub-state selection ----
        // PoweredOn -> Active/Idle on entry (handled via initial)
        // Active/Idle -> Standby
        add_rule(DeviceState::ActiveIdle, typeid(Standby),
                 DeviceState::Standby);
        // Standby -> Active/Idle
        add_rule(DeviceState::Standby, typeid(WakeUp), DeviceState::ActiveIdle);

        // ---- Level 2: Active sub-states ----
        add_rule(DeviceState::ActiveIdle, typeid(Heartbeat),
                 DeviceState::Running, {},
                 [this](const EventEnvelope&) { metrics.total_transitions++; });
        add_rule(DeviceState::Running, typeid(Heartbeat), DeviceState::Busy);
        add_rule(DeviceState::Busy, typeid(Heartbeat), DeviceState::ActiveIdle);

        // Over-temperature detection (from any Active sub-state)
        for (auto s :
             {DeviceState::ActiveIdle, DeviceState::Running, DeviceState::Busy})
        {
            add_rule(s, typeid(OverTemperature), DeviceState::MinorFault, {},
                     [this](const EventEnvelope& evt) {
                         auto& e = evt.get<OverTemperature>();
                         metrics.max_temperature = e.temp;
                     });
        }

        // ---- Level 2: Fault -> Recovery ----
        add_rule(DeviceState::MinorFault, typeid(RecoveryInitiated),
                 DeviceState::Recovering);
        add_rule(DeviceState::MajorFault, typeid(RecoveryInitiated),
                 DeviceState::Recovering);

        // Auto-recovery from minor fault
        add_rule(DeviceState::MinorFault, typeid(NormalTemperature),
                 DeviceState::ActiveIdle, {}, [this](const EventEnvelope&) {
                     log.info(
                         "  [Fault] Temperature normalized, auto-recovered");
                 });

        // Recovery -> Active (with history)
        add_rule(DeviceState::Recovering, typeid(RecoveryComplete),
                 DeviceState::ActiveIdle, {}, [this](const EventEnvelope&) {
                     log.info(
                         "  [Fault] Recovery complete, resuming operation");
                 });

        // Recovery failure -> MajorFault
        add_rule(DeviceState::Recovering, typeid(FaultDetected),
                 DeviceState::MajorFault, {}, [this](const EventEnvelope&) {
                     log.error(
                         "  [Fault] Recovery failed, escalated to MajorFault");
                 });

        // Fault escalation
        add_rule(DeviceState::MinorFault, typeid(FaultDetected),
                 DeviceState::MajorFault, {}, [this](const EventEnvelope&) {
                     log.error("  [Fault] Escalated to MajorFault");
                 });

        // MajorFault -> PowerOff
        add_rule(DeviceState::MajorFault, typeid(PowerOff),
                 DeviceState::PoweredOff, {}, [this](const EventEnvelope&) {
                     log.warn("  [Fault] Major fault: powering off");
                 });

        // ---- Firmware Update Flow ----
        add_rule(
            DeviceState::ActiveIdle, typeid(StartFirmwareUpdate),
            DeviceState::FWDownloading, {}, [this](const EventEnvelope& evt) {
                auto& e = evt.get<StartFirmwareUpdate>();
                log.info("  [FW] Starting update to version %s (%zu bytes)",
                         e.version, e.size_bytes);
                metrics.fw_update_count++;
            });

        add_rule(DeviceState::FWDownloading, typeid(FWDownloadComplete),
                 DeviceState::FWVerifying, {}, [this](const EventEnvelope&) {
                     log.info("  [FW] Download complete, verifying");
                 });

        add_rule(DeviceState::FWVerifying, typeid(FWUpdateComplete),
                 DeviceState::FWInstalling, {}, [this](const EventEnvelope&) {
                     log.info("  [FW] Verified OK, installing");
                 });

        add_rule(DeviceState::FWVerifying, typeid(FWVerifyFailure),
                 DeviceState::ActiveIdle, {}, [this](const EventEnvelope& evt) {
                     auto& e = evt.get<FWVerifyFailure>();
                     log.error("  [FW] Verification failed: %s", e.detail);
                 });

        add_rule(DeviceState::FWInstalling, typeid(FWUpdateComplete),
                 DeviceState::ActiveIdle, {}, [this](const EventEnvelope&) {
                     log.info("  [FW] Update successful, restarting");
                 });

        add_rule(DeviceState::FWInstalling, typeid(FWUpdateFailed),
                 DeviceState::MajorFault, {}, [this](const EventEnvelope&) {
                     log.error("  [FW] Install failed, entering fault state");
                 });
    }

    void normal_operation()
    {
        engine.handle(PowerOn{});
        engine.handle(Heartbeat{1000}); // ActiveIdle -> Running
        engine.handle(Heartbeat{1001}); // Running -> Busy
        engine.handle(Heartbeat{1002}); // Busy -> ActiveIdle
    }

    void fault_recovery()
    {
        engine.handle(OverTemperature{85.5});
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        engine.handle(NormalTemperature{});
    }

    void firmware_update()
    {
        StartFirmwareUpdate fw_evt;
        std::strncpy(fw_evt.version, "v2.1.0", sizeof(fw_evt.version) - 1);
        fw_evt.size_bytes = 16777216; // 16MB

        engine.handle(fw_evt);
        engine.handle(FWDownloadComplete{});
        engine.handle(FWUpdateComplete{});
        engine.handle(FWUpdateComplete{});
    }

    void power_cycle_with_history()
    {
        engine.handle(PowerOff{});
        engine.handle(PowerOn{}); // Should go to Active/Idle (history)
    }

    void print_metrics()
    {
        std::printf("\n--- Device Metrics ---\n");
        std::printf("Faults:         %d\n", metrics.fault_count);
        std::printf("Recoveries:     %d\n", metrics.recovery_count);
        std::printf("FW Updates:     %d\n", metrics.fw_update_count);
        std::printf("Max Temp:       %.1f C\n", metrics.max_temperature);
        std::printf("Total Trans:    %d\n", metrics.total_transitions);
        std::printf("Final State:    %s\n",
                    engine.get_state_name(engine.current_state()));
    }
};

int main()
{
    Logger::instance().set_level(LogLevel::Debug);
    DeviceManager().run();
    return 0;
}
