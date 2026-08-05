// 数组/容器越界 — C++ 面试崩溃用例 #08
// 编译: g++ -g -o 08_out_of_bounds 08_out_of_bounds.cpp
// ASan: g++ -fsanitize=address -g -o 08_out_of_bounds 08_out_of_bounds.cpp

#include <iostream>
#include <vector>
#include <string>

// ❌ 崩溃：C 风格数组越界
void crash_case_1() {
    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << arr[10] << "\n";     // 越界读取
    arr[10] = 99;                     // 越界写入
}

// ❌ 崩溃：vector 下标越界 (operator[] 不检查)
void crash_case_2() {
    std::vector<int> v = {1, 2, 3};
    v[5] = 10;                        // 越界，静默损坏或崩溃
    std::cout << v[5] << "\n";
}

// ❌ 崩溃：string 越界访问
void crash_case_3() {
    std::string s = "hello";
    char c = s[100];                  // 越界
    std::cout << c << "\n";
}

// ❌ 崩溃：at() 会抛异常但常被忽略
void crash_case_4() {
    std::vector<int> v = {1, 2, 3};
    try {
        std::cout << v.at(10) << "\n";   // 抛 std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    // crash_case_3();
    // crash_case_4();
    std::cout << "uncomment a case to test\n";
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或 std::out_of_range 异常
 *
 * GDB 定位:
 *   ASan 最精准，直接报越界位置和大小
 *
 * ASan 输出:
 *   ERROR: AddressSanitizer: heap-buffer-overflow on address 0xADDR
 *
 * operator[] vs at():
 *   v[i]   — 不检查边界，性能高，越界是 UB
 *   v.at(i) — 检查边界，越界抛 std::out_of_range
 *
 * 修复方案:
 *   1. 用 at() 替代 operator[] (调试阶段)
 *   2. 用迭代器 + begin()/end() 遍历
 *   3. 用 range-based for 循环
 *   4. 用 std::span (C++20) 做安全视图
 *   5. CI 中跑 ASan
 */
