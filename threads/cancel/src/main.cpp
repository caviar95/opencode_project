#include "stage/pipeline.hpp"

#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

static const char* stage_name(int i) {
    static const char* names[] = {"step1", "step2", "step3", "step4", "step5"};
    return (i >= 0 && i < 5) ? names[i] : "?";
}

static stage::Pipeline::Step make_step(int i, int ms) {
    return [i, ms](stage::Pipeline&) {
        printf("    [%s] begin (%d ms, runs to completion)\n", stage_name(i), ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        printf("    [%s] done\n", stage_name(i));
    };
}

int main() {
    puts("Scenario 1: stop RPC arrives in the middle of step2.\n"
         "            step2 is NOT killed; it finishes, steps 3-5 are skipped,\n"
         "            then cleanup runs.");
    {
        stage::Pipeline pipe;
        pipe.set_steps({
            make_step(0, 300),
            make_step(1, 400),
            make_step(2, 500),
            make_step(3, 200),
            make_step(4, 300),
        });
        pipe.set_cleanup([](stage::Pipeline&, bool aborted) {
            printf("    [cleanup] begin (aborted=%d)\n", (int)aborted);
            std::this_thread::sleep_for(150ms);
            printf("    [cleanup] done\n");
        });

        pipe.start();

        // Simulated "stop" RPC arriving at t=550ms, mid-step2.
        std::thread rpc([&pipe] {
            std::this_thread::sleep_for(550ms);
            printf("\n  >>> stop RPC arrives, stage %s executing\n",
                   stage_name(pipe.current_stage()));
            auto t0 = std::chrono::steady_clock::now();
            pipe.stop();
            auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - t0);
            printf("  >>> stop() returned after %lld ms, pipeline fully stopped\n",
                   (long long)dt.count());
        });

        pipe.wait();
        rpc.join();
        pipe.rethrow_if_failed();
    }

    puts("\nScenario 2: cooperative stage. The stage may end early, but only\n"
         "            when IT chooses to check the flag.");
    {
        stage::Pipeline pipe;
        pipe.set_steps({
            [](stage::Pipeline&) {
                printf("    [step1] begin/done\n");
                std::this_thread::sleep_for(100ms);
            },
            [](stage::Pipeline& p) {
                printf("    [step2] long cooperative stage begin\n");
                for (int i = 0; i < 100; ++i) {
                    if (p.stop_requested()) {
                        printf("    [step2] exited early (cooperative)\n");
                        return;
                    }
                    std::this_thread::sleep_for(50ms);
                }
                printf("    [step2] completed naturally\n");
            },
            [](stage::Pipeline&) { printf("    [step3] must NOT run\n"); },
        });

        pipe.start();
        std::thread rpc([&pipe] {
            std::this_thread::sleep_for(180ms);
            pipe.stop();
        });
        pipe.wait();
        rpc.join();
        pipe.rethrow_if_failed();
    }

    puts("\nall scenarios finished");
    return 0;
}
