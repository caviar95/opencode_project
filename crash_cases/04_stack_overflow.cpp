// 栈溢出 — C++ 面试崩溃用例 #04
// 编译: g++ -g -o 04_stack_overflow 04_stack_overflow.cpp

#include <iostream>

// ❌ 崩溃：无限递归
void crash_case_1(int n) {
    std::cout << n << "\n";
    crash_case_1(n + 1);       // 无终止条件，栈耗尽
}

// ❌ 崩溃：超大栈分配
void crash_case_2() {
    char buf[10 * 1024 * 1024];  // 10MB，超过默认栈大小 (通常 8MB)
    buf[0] = 'a';
}

// ❌ 崩溃：递归深度过大
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);  // 指数级栈增长
}

void crash_case_3() {
    std::cout << fibonacci(100) << "\n";   // 栈溢出
}

int main() {
    // 取消注释逐一测试
    // crash_case_1(0);
    // crash_case_2();
    // crash_case_3();
    std::cout << "uncomment a case to test\n";
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或 SIGBUS
 *
 * GDB 定位:
 *   gdb -ex run -ex bt ./04_stack_overflow
 *   bt 会看到同一函数重复出现数千次
 *
 * 特征:
 *   崩溃地址通常在栈底附近 (如 0x7fffxxxxx 接近栈边界)
 *
 * 修复方案:
 *   1. 递归改迭代 (尾递归优化并非所有编译器都支持)
 *   2. 大对象分配到堆 (用 vector/new)
 *   3. 限制递归深度
 *   4. ulimit -s 查看/调整栈大小
 *   5. 用 BFS 替代 DFS (减少栈深度)
 */
