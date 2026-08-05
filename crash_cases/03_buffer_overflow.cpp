// 缓冲区溢出 — C++ 面试崩溃用例 #03
// 编译: g++ -g -o 03_buffer_overflow 03_buffer_overflow.cpp
// ASan: g++ -fsanitize=address -g -o 03_buffer_overflow 03_buffer_overflow.cpp

#include <iostream>
#include <cstring>

// ❌ 崩溃：栈缓冲区溢出
void crash_case_1() {
    char buf[4];
    strcpy(buf, "this is a very long string");  // 溢出，覆盖栈上其他数据
    std::cout << buf << "\n";
}

// ❌ 崩溃：堆缓冲区溢出
void crash_case_2() {
    int* arr = new int[5];
    arr[10] = 42;              // 越界写入，破坏堆元数据
    delete[] arr;              // 可能在 delete 时崩溃
}

// ❌ 崩溃：off-by-one 经典错误
void crash_case_3() {
    char buf[5];
    for (int i = 0; i <= 5; i++) {  // 应该是 < 5
        buf[i] = 'a';
    }
    buf[5] = '\0';               // off-by-one 写入
}

// ❌ 崩溃：格式化字符串漏洞 + 溢出
void crash_case_4() {
    char dst[8];
    char src[] = "hello world overflow!";
    memcpy(dst, src, strlen(src));   // 目标缓冲区太小
    std::cout << dst << "\n";
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
 * 崩溃信号: SIGSEGV (11) 或 SIGABRT (6)
 *
 * GDB 定位:
 *   栈溢出可能覆盖返回地址，导致跳到非法地址
 *   bt 会看到奇怪的函数名或 ??
 *
 * ASan 输出:
 *   ERROR: AddressSanitizer: heap-buffer-overflow
 *   ERROR: AddressSanitizer: stack-buffer-overflow
 *
 * 修复方案:
 *   1. 用 std::string / std::vector 替代裸数组
 *   2. 用 strncpy / snprintf 替代 strcpy / sprintf
 *   3. 用 std::array / std::span (C++20) 做边界检查
 *   4. 编译加 -fstack-protector-all (栈保护)
 *   5. CI 中跑 ASan
 */
