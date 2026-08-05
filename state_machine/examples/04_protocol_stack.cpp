#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <hfsm/core/region.hpp>
#include <hfsm/ext/logger.hpp>
#include <hfsm/hfsm.hpp>

using namespace hfsm;

// ============================================================
// Example 4: Protocol Stack with HFSM
//
// Demonstrates hierarchical state machines:
// - Top level: Connection (Disconnected, Connecting, Connected)
// - Sub-states within Connected (Authenticating, Idle, Streaming)
// - Orthogonal regions for RX and TX paths
// ============================================================

// ---- Events ----

// Connection-level events
struct ConnectRequest
{
    char host[128];
    int port;
};
struct DisconnectRequest
{
    int reason;
};
struct ConnectionTimeout
{
};
struct ConnectionEstablished
{
};
struct ConnectionLost
{
    int error;
};

// Authentication events
struct Authenticate
{
    char token[128];
};
struct AuthSuccess
{
    char session_id[64];
};
struct AuthFailure
{
    int code;
};

// Stream events
struct DataReceived
{
    int len;
    char data[1024];
};
struct DataSent
{
    int bytes;
};
struct StreamStart
{
};
struct StreamStop
{
};

// ---- State Hierarchy ----
//
// Connection (top)
//   |-- Disconnected
//   |-- Connecting
//   |-- Connected (composite)
//         |-- Authenticating
//         |-- Idle
//         |-- Streaming
//
// Orthogonal regions:
//   - RX Path: Idle -> Receiving -> Processing
//   - TX Path: Idle -> Sending -> WaitingForAck

enum class ConnState : StateId {
    Disconnected,
    Connecting,
    Connected,
    // Sub-states of Connected:
    Authenticating,
    Idle,
    Streaming,
};

const char* conn_state_name(ConnState s)
{
    switch (s) {
    case ConnState::Disconnected:
        return "Disconnected";
    case ConnState::Connecting:
        return "Connecting";
    case ConnState::Connected:
        return "Connected";
    case ConnState::Authenticating:
        return "Authenticating";
    case ConnState::Idle:
        return "Idle";
    case ConnState::Streaming:
        return "Streaming";
    }
    return "UNKNOWN";
}

// Protocol stack statistics
struct ProtocolStats
{
    int bytes_sent = 0;
    int bytes_received = 0;
    int packets_sent = 0;
    int packets_recv = 0;
    int auth_failures = 0;
    int reconnects = 0;
};

class ProtocolStack
{
public:
    StateMachineEngine engine;
    RegionManager regions;
    ProtocolStats stats;
    ModuleLogger log{"ProtocolStack"};

    ProtocolStack() : log("ProtocolStack")
    {
        // Register all states
        for (int i = 0; i <= static_cast<int>(ConnState::Streaming); i++) {
            auto s = static_cast<ConnState>(i);
            engine.register_state(static_cast<StateId>(s));
            engine.set_state_name(static_cast<StateId>(s), conn_state_name(s));
        }

        engine.set_initial(static_cast<StateId>(ConnState::Disconnected));

        // ---- Entry/Exit Callbacks ----
        engine.on_entry(static_cast<StateId>(ConnState::Disconnected),
                        [this](const auto&) { log.info("Socket closed"); });
        engine.on_entry(
            static_cast<StateId>(ConnState::Connecting),
            [this](const auto&) { log.info("Attempting connection..."); });
        engine.on_entry(
            static_cast<StateId>(ConnState::Connected), [this](const auto&) {
                log.info("Connection established, entering sub-states");
                regions.activate_region(
                    static_cast<StateId>(ConnState::Connected));
            });
        engine.on_entry(static_cast<StateId>(ConnState::Authenticating),
                        [this](const auto&) { log.info("Authenticating..."); });
        engine.on_entry(static_cast<StateId>(ConnState::Idle),
                        [this](const auto&) {
                            log.info("Connection idle, waiting for data");
                        });
        engine.on_entry(static_cast<StateId>(ConnState::Streaming),
                        [this](const auto&) { log.info("Streaming data"); });

        // ---- Top-Level Transitions ----
        auto sid = [](ConnState s) { return static_cast<StateId>(s); };

        // Disconnected -> Connecting
        engine.add_rule({sid(ConnState::Disconnected),
                         typeid(ConnectRequest),
                         sid(ConnState::Connecting),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<ConnectRequest>();
                             log.info("Connecting to %s:%d", e.host, e.port);
                         }});

        // Disconnected -> Connecting (reconnect after connection lost)
        engine.add_rule({sid(ConnState::Disconnected),
                         typeid(ConnectionLost),
                         sid(ConnState::Connecting),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope&) {
                             stats.reconnects++;
                             log.warn("Reconnecting (attempt %d)",
                                      stats.reconnects);
                         }});

        // Connecting -> Connected (success)
        engine.add_rule({sid(ConnState::Connecting),
                         typeid(ConnectionEstablished),
                         sid(ConnState::Connected)});

        // Connecting -> Disconnected (timeout)
        engine.add_rule({sid(ConnState::Connecting),
                         typeid(ConnectionTimeout),
                         sid(ConnState::Disconnected),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope&) {
                             log.error("Connection timed out");
                         }});

        // Connected -> Disconnected (on disconnect or lost)
        engine.add_rule({sid(ConnState::Connected),
                         typeid(DisconnectRequest),
                         sid(ConnState::Disconnected),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<DisconnectRequest>();
                             log.info("Disconnected (reason: %d)", e.reason);
                         }});
        engine.add_rule(
            {sid(ConnState::Connected),
             typeid(ConnectionLost),
             sid(ConnState::Disconnected),
             false,
             false,
             {},
             [this](const EventEnvelope&) { log.error("Connection lost"); }});

        // ---- Sub-state Transitions within Connected ----
        // Connected/Authenticating -> Connected/Authenticating -> Idle
        engine.add_rule({sid(ConnState::Authenticating),
                         typeid(AuthSuccess),
                         sid(ConnState::Idle),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<AuthSuccess>();
                             log.info("Authentication success (session: %s)",
                                      e.session_id);
                         }});

        // Connected/Authenticating -> Connected/Authenticating -> Disconnected
        engine.add_rule({sid(ConnState::Authenticating),
                         typeid(AuthFailure),
                         sid(ConnState::Disconnected),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope&) {
                             stats.auth_failures++;
                             log.error("Authentication failure");
                         }});

        // Connected/Idle -> Connected/Idle -> Streaming
        engine.add_rule({sid(ConnState::Idle), typeid(StreamStart),
                         sid(ConnState::Streaming)});

        // Connected/Streaming -> Connected/Streaming -> Idle
        engine.add_rule({sid(ConnState::Streaming), typeid(StreamStop),
                         sid(ConnState::Idle)});

        // Data events in Idle
        engine.add_rule({sid(ConnState::Idle),
                         typeid(DataReceived),
                         sid(ConnState::Idle),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<DataReceived>();
                             stats.bytes_received += e.len;
                             stats.packets_recv++;
                             log.debug("Received %d bytes (total: %d)", e.len,
                                       stats.bytes_received);
                         }});

        // Data events in Streaming
        engine.add_rule({sid(ConnState::Streaming),
                         typeid(DataReceived),
                         sid(ConnState::Streaming),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<DataReceived>();
                             stats.bytes_received += e.len;
                             stats.packets_recv++;
                             log.debug("Streaming: received %d bytes", e.len);
                         }});

        engine.add_rule({sid(ConnState::Streaming),
                         typeid(DataSent),
                         sid(ConnState::Streaming),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<DataSent>();
                             stats.bytes_sent += e.bytes;
                             stats.packets_sent++;
                             log.debug("Streaming: sent %d bytes", e.bytes);
                         }});
    }

    void run()
    {
        std::printf("\n=== Protocol Stack Simulation ===\n\n");

        // Connection lifecycle
        engine.handle(ConnectRequest{"api.example.com", 443});
        engine.handle(ConnectionTimeout{}); // First attempt times out

        engine.handle(ConnectRequest{"api.example.com", 443});
        engine.handle(ConnectionEstablished{});

        // Authenticate
        engine.handle(AuthSuccess{"session-abc-123"});

        // Idle: receive some data
        engine.handle(DataReceived{64, "ping"});
        engine.handle(DataReceived{128, "more data"});

        // Start streaming
        engine.handle(StreamStart{});
        engine.handle(DataReceived{1024, "stream chunk 1"});
        engine.handle(DataSent{512});
        engine.handle(DataReceived{2048, "stream chunk 2"});
        engine.handle(StreamStop{});

        // Graceful disconnect
        engine.handle(DisconnectRequest{0});

        std::printf("\n--- Protocol Stack Stats ---\n");
        std::printf("Bytes sent:     %d\n", stats.bytes_sent);
        std::printf("Bytes received: %d\n", stats.bytes_received);
        std::printf("Packets sent:   %d\n", stats.packets_sent);
        std::printf("Packets recv:   %d\n", stats.packets_recv);
        std::printf("Reconnects:     %d\n", stats.reconnects);

        std::printf("\nFinal state: %s\n",
                    engine.get_state_name(engine.current_state()));
    }
};

int main()
{
    Logger::instance().set_level(LogLevel::Info);
    ProtocolStack().run();
    return 0;
}
