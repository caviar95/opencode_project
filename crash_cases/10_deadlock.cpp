// 死锁 (Deadlock) — C++ 面试崩溃用例 #10
// 编译: g++ -g -std=c++17 -pthread -o 10_deadlock 10_deadlock.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx_a;
std::mutex mtx_b;

// ❌ 死锁：两个线程以不同顺序获取锁
void thread_1() {
    mtx_a.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 增大死锁概率
    mtx_b.lock();                    // 等 mtx_b，但 thread_2 持有它
    std::cout << "thread_1 got both locks\n";
    mtx_b.unlock();
    mtx_a.unlock();
}

void thread_2() {
    mtx_b.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mtx_a.lock();                    // 等 mtx_a，但 thread_1 持有它
    std::cout << "thread_2 got both locks\n";
    mtx_a.unlock();
    mtx_b.unlock();
}

void crash_case_1() {
    std::thread t1(thread_1);
    std::thread t2(thread_2);
    t1.join();
    t2.join();                       // 永远不会到这里
}

// ❌ 死锁：同一线程递归锁 (mutex 不支持递归)
std::mutex non_recursive;

void inner() {
    non_recursive.lock();
    std::cout << "inner\n";
    non_recursive.unlock();
}

void outer() {
    non_recursive.lock();
    inner();                         // inner 再次 lock 同一个 mutex → 死锁
    non_recursive.unlock();
}

void crash_case_2() {
    outer();
}

// ✅ 修复：用 std::lock 同时获取多个锁
void safe_thread_1() {
    std::lock(mtx_a, mtx_b);
    std::lock_guard<std::mutex> lock_a(mtx_a, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(mtx_b, std::adopt_lock);
    std::cout << "safe_thread_1 got both locks\n";
}

void safe_thread_2() {
    std::lock(mtx_b, mtx_a);         // 顺序无关，std::lock 避免死锁
    std::lock_guard<std::mutex> lock_b(mtx_b, std::adopt_lock);
    std::lock_guard<std::mutex> lock_a(mtx_a, std::adopt_lock);
    std::cout << "safe_thread_2 got both locks\n";
}

void fix_case() {
    std::thread t1(safe_thread_1);
    std::thread t2(safe_thread_2);
    t1.join();
    t2.join();
}

// ✅ 修复：用 std::scoped_lock (C++17 推荐)
void fix_case_cpp17() {
    auto worker = [](const char* name) {
        std::scoped_lock lock(mtx_a, mtx_b);  // 自动避免死锁
        std::cout << name << " got both locks\n";
    };
    std::thread t1(worker, "t1");
    std::thread t2(worker, "t2");
    t1.join();
    t2.join();
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();              // 会挂起
    // crash_case_2();
    // fix_case();
    fix_case_cpp17();
}

/*
 * ====== 面试要点 ======
 *
 * 表现: 进程不崩溃但卡死，CPU 可能很低 (线程在等待)
 *
 * GDB 定位:
 *   gdb -p <pid>
 *   thread apply all bt               # 看所有线程栈
 *   info mutex (gdb 9.2+)             # 看锁状态
 *
 * 死锁四条件 (Coffman):
 *   1. 互斥        — 资源不可共享
 *   2. 持有并等待   — 持有一个锁等另一个
 *   3. 不可抢占     — 锁不能被强制释放
 *   4. 循环等待     — A→B→A 的依赖环
 *
 * 修复方案:
 *   1. 统一加锁顺序 (破坏循环等待)
 *   2. std::lock / std::scoped_lock 同时获取多个锁
 *   3. 用 std::recursive_mutex (但需谨慎)
 *   4. 用 std::try_lock + 超时 (破坏不可抢占)
 *   5. 减少锁粒度，缩小临界区
 *   6. 用无锁数据结构或 actor 模型
 */
