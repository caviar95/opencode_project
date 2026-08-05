// 数据竞争 (Data Race) — C++ 面试崩溃用例 #09
// 编译: g++ -g -std=c++17 -pthread -o 09_data_race 09_data_race.cpp
// TSan: g++ -fsanitize=thread -g -std=c++17 -pthread -o 09_data_race 09_data_race.cpp

#include <iostream>
#include <thread>
#include <vector>

// ❌ 崩溃：多线程同时写共享变量 (无保护)
int shared_counter = 0;

void increment(int id, int iterations) {
    for (int i = 0; i < iterations; i++) {
        shared_counter++;          // 非原子操作，数据竞争
    }
}

void crash_case_1() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment, i, 10000);
    }
    for (auto& t : threads) t.join();

    // 结果几乎不会是 100000
    std::cout << "counter = " << shared_counter << "\n";
}

// ❌ 崩溃：多线程同时修改容器
void crash_case_2() {
    std::vector<int> v;

    auto writer = [&]() {
        for (int i = 0; i < 1000; i++) {
            v.push_back(i);        // 和 reader 并发
        }
    };

    auto reader = [&]() {
        for (int i = 0; i < 1000; i++) {
            if (!v.empty()) {
                std::cout << v.back() << "\n";  // 可能读到无效数据
            }
        }
    };

    std::thread t1(writer);
    std::thread t2(reader);
    t1.join();
    t2.join();
}

// ✅ 修复：用 mutex 保护
#include <mutex>
std::mutex mtx;
int safe_counter = 0;

void safe_increment(int iterations) {
    for (int i = 0; i < iterations; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        safe_counter++;
    }
}

void fix_case() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(safe_increment, 10000);
    }
    for (auto& t : threads) t.join();
    std::cout << "safe_counter = " << safe_counter << "\n";  // 正确: 100000
}

// ✅ 修复：用 atomic
#include <atomic>
std::atomic<int> atomic_counter{0};

void atomic_increment(int iterations) {
    for (int i = 0; i < iterations; i++) {
        atomic_counter++;
    }
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    fix_case();
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或静默数据损坏
 *
 * 特征:
 *   难以复现，只在特定调度下出现 (heisenbug)
 *   加了 printf 可能就不出现了
 *
 * TSan 输出:
 *   WARNING: ThreadSanitizer: data race
 *
 * GDB 定位:
 *   多线程 gdb attach，看各线程状态
 *   thread apply all bt
 *
 * 修复方案:
 *   1. 用 std::mutex + lock_guard 保护共享数据
 *   2. 用 std::atomic (简单类型)
 *   3. 避免共享，用 thread-local 或消息传递
 *   4. 用 RAII 锁管理，避免忘记 unlock
 *   5. 定期跑 TSan
 */
