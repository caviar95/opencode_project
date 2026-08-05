#include <cstdio>
#include <cstring>
#include <hfsm/hfsm.hpp>

using namespace hfsm;

// ============================================================
// Example 2: TCP Connection State Machine (RFC 793)
//
// States: CLOSED -> LISTEN -> SYN_SENT/SYN_RCVD -> ESTABLISHED
//         -> FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
//         -> CLOSE_WAIT -> LAST_ACK -> CLOSED
//
// This demonstrates a real-world protocol state machine
// ============================================================

// TCP Events
struct PassiveOpen
{
};
struct ActiveOpen
{
};
struct SendSYN
{
};
struct ReceiveSYN
{
    uint32_t seq_num;
};
struct ReceiveSYNACK
{
    uint32_t seq_num;
    uint32_t ack_num;
};
struct ReceiveACK
{
    uint32_t ack_num;
};
struct Close
{
};
struct ReceiveFIN
{
};
struct Timeout
{
};
struct SendRST
{
};

// Connection metadata
struct ConnectionInfo
{
    uint32_t seq_num = 1000;
    uint32_t ack_num = 0;
    int retransmit_count = 0;
    bool has_data = false;

    void reset()
    {
        seq_num = 1000;
        ack_num = 0;
        retransmit_count = 0;
        has_data = false;
    }
};

// TCP States (RFC 793 condensed)
enum class TCPState : StateId {
    Closed,
    Listen,
    SYN_Sent,
    SYN_RCVD,
    Established,
    FIN_Wait_1,
    FIN_Wait_2,
    Time_Wait,
    Close_Wait,
    Last_ACK,
};

const char* tcp_state_name(TCPState s)
{
    switch (s) {
    case TCPState::Closed:
        return "CLOSED";
    case TCPState::Listen:
        return "LISTEN";
    case TCPState::SYN_Sent:
        return "SYN_SENT";
    case TCPState::SYN_RCVD:
        return "SYN_RCVD";
    case TCPState::Established:
        return "ESTABLISHED";
    case TCPState::FIN_Wait_1:
        return "FIN_WAIT_1";
    case TCPState::FIN_Wait_2:
        return "FIN_WAIT_2";
    case TCPState::Time_Wait:
        return "TIME_WAIT";
    case TCPState::Close_Wait:
        return "CLOSE_WAIT";
    case TCPState::Last_ACK:
        return "LAST_ACK";
    }
    return "UNKNOWN";
}

class TCPConnection
{
public:
    StateMachineEngine engine;
    ConnectionInfo info;

    TCPConnection()
    {
        // Register all states
        for (int i = 0; i <= static_cast<int>(TCPState::Last_ACK); i++) {
            auto s = static_cast<TCPState>(i);
            engine.register_state(static_cast<StateId>(s));
            engine.set_state_name(static_cast<StateId>(s), tcp_state_name(s));
        }

        engine.set_initial(static_cast<StateId>(TCPState::Closed));

        // ---- CLOSED transitions ----
        add_rule(TCPState::Closed, typeid(PassiveOpen), TCPState::Listen);
        add_rule(TCPState::Closed, typeid(ActiveOpen), TCPState::SYN_Sent, {},
                 [this](auto&) {
                     std::printf("[TCP] Active open: sending SYN (seq=%u)\n",
                                 info.seq_num);
                 });

        // ---- LISTEN transitions ----
        add_rule(
            TCPState::Listen, typeid(ReceiveSYN), TCPState::SYN_RCVD, {},
            [this](const EventEnvelope& evt) {
                auto& syn = evt.get<ReceiveSYN>();
                info.ack_num = syn.seq_num + 1;
                std::printf(
                    "[TCP] LISTEN: received SYN (seq=%u), sending SYN-ACK\n",
                    syn.seq_num);
            });

        // ---- SYN_SENT transitions ----
        add_rule(TCPState::SYN_Sent, typeid(ReceiveSYNACK),
                 TCPState::Established, {}, [this](const EventEnvelope& evt) {
                     auto& synack = evt.get<ReceiveSYNACK>();
                     info.ack_num = synack.seq_num + 1;
                     std::printf(
                         "[TCP] SYN_SENT: received SYN-ACK (seq=%u, ack=%u), "
                         "connection established\n",
                         synack.seq_num, synack.ack_num);
                 });

        // ---- SYN_RCVD transitions ----
        add_rule(TCPState::SYN_RCVD, typeid(ReceiveACK), TCPState::Established,
                 {}, [this](const EventEnvelope& evt) {
                     auto& ack = evt.get<ReceiveACK>();
                     std::printf("[TCP] SYN_RCVD: received ACK (%u), "
                                 "connection established\n",
                                 ack.ack_num);
                 });

        // ---- ESTABLISHED transitions ----
        add_rule(TCPState::Established, typeid(Close), TCPState::FIN_Wait_1, {},
                 [this](auto&) {
                     std::printf("[TCP] Close requested: sending FIN\n");
                 });

        add_rule(TCPState::Established, typeid(ReceiveFIN),
                 TCPState::Close_Wait, {}, [this](auto&) {
                     std::printf("[TCP] Received FIN: entering CLOSE_WAIT\n");
                 });

        // ---- FIN_WAIT_1 transitions ----
        add_rule(TCPState::FIN_Wait_1, typeid(ReceiveACK),
                 TCPState::FIN_Wait_2);
        add_rule(TCPState::FIN_Wait_1, typeid(ReceiveFIN), TCPState::Time_Wait);

        // ---- FIN_WAIT_2 transitions ----
        add_rule(
            TCPState::FIN_Wait_2, typeid(ReceiveFIN), TCPState::Time_Wait, {},
            [this](auto&) {
                std::printf(
                    "[TCP] Received FIN in FIN_WAIT_2: entering TIME_WAIT\n");
            });

        // ---- TIME_WAIT transitions ----
        add_rule(TCPState::Time_Wait, typeid(Timeout), TCPState::Closed, {},
                 [this](auto&) {
                     std::printf(
                         "[TCP] TIME_WAIT expired: connection closed\n");
                 });

        // ---- CLOSE_WAIT transitions ----
        add_rule(TCPState::Close_Wait, typeid(Close), TCPState::Last_ACK, {},
                 [this](auto&) {
                     std::printf("[TCP] CLOSE_WAIT: close requested, sending "
                                 "FIN (LAST_ACK)\n");
                 });

        // ---- LAST_ACK transitions ----
        add_rule(TCPState::Last_ACK, typeid(ReceiveACK), TCPState::Closed, {},
                 [this](auto&) {
                     std::printf(
                         "[TCP] LAST_ACK: received ACK, connection closed\n");
                 });
    }

    void run_active_open()
    {
        std::printf("\n=== TCP Active Open Scenario ===\n\n");

        engine.handle(ActiveOpen{});
        engine.handle(ReceiveSYNACK{2000, 1001});
        info.seq_num++;
        engine.handle(ReceiveACK{2001});

        std::printf("\n--- Data transfer phase ---\n");
        engine.handle(Close{});

        // Simulate ACK for FIN
        engine.handle(ReceiveACK{2002});
        engine.handle(ReceiveFIN{});

        std::printf("\n--- Waiting for TIME_WAIT timeout ---\n");
        engine.handle(Timeout{});
    }

    void run_passive_open()
    {
        std::printf("\n=== TCP Passive Open Scenario ===\n\n");
        engine.reset();

        engine.handle(PassiveOpen{});
        engine.handle(ReceiveSYN{5000});
        engine.handle(ReceiveACK{5001});

        std::printf("\n--- Passive close ---\n");
        engine.handle(ReceiveFIN{});
        engine.handle(Close{});
        engine.handle(ReceiveACK{5002});
    }

private:
    void add_rule(TCPState src,
                  const std::type_index& evt,
                  TCPState dst,
                  std::function<bool(const EventEnvelope&)> guard = {},
                  std::function<void(const EventEnvelope&)> action = {})
    {
        engine.add_rule({static_cast<StateId>(src), evt,
                         static_cast<StateId>(dst), false, false,
                         std::move(guard), std::move(action)});
    }
};

int main()
{
    TCPConnection conn;
    conn.run_active_open();
    std::printf("\n");
    conn.run_passive_open();
    return 0;
}
