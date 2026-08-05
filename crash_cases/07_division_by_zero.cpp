// 除零错误 — C++ 面试崩溃用例 #07
// 编译: g++ -g -o 07_division_by_zero 07_division_by_zero.cpp

#include <iostream>

// ❌ 崩溃：整数除零
void crash_case_1() {
    int a = 10;
    int b = 0;
    std::cout << a / b << "\n";   // SIGFPE
}

// ❌ 崩溃：取模除零
void crash_case_2() {
    int x = 10;
    int y = 0;
    std::cout << x % y << "\n";   // SIGFPE
}

// ❌ 崩溃：间接除零 (参数传递)
int compute_ratio(int total, int count) {
    return total / count;          // count 可能为 0
}

void crash_case_3() {
    std::cout << compute_ratio(100, 0) << "\n";
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    // crash_case_3();
    std::cout << "uncomment a case to test\n";
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGFPE (8) — Floating Point Exception (包括整数除零)
 *
 * GDB 定位:
 *   gdb -ex run -ex bt ./07_division_by_zero
 *   会精确停在除法指令处
 *
 * 注意: 浮点数除零不会崩溃，结果是 inf/nan
 *   double x = 1.0 / 0.0;   // x == inf，不崩溃
 *
 * 修复方案:
 *   1. 除法前检查分母: if (b == 0) return 0;
 *   2. 使用 std::optional 表示可能无效的结果
 *   3. 断言: assert(b != 0);
 *   4. 代码审查 + 静态分析
 */
