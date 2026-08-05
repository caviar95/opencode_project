// 未初始化指针 — C++ 面试崩溃用例 #06
// 编译: g++ -g -o 06_uninitialized_ptr 06_uninitialized_ptr.cpp
// UBSan: g++ -fsanitize=undefined -g -o 06_uninitialized_ptr 06_uninitialized_ptr.cpp

#include <iostream>

// ❌ 崩溃：未初始化的局部指针
void crash_case_1() {
    int* p;                    // 值是随机的（栈上残留）
    *p = 42;                   // 可能写到一个非法地址
}

// ❌ 崩溃：未初始化的成员指针
struct Connection {
    int* socket;
    int port;

    Connection(int p) : port(p) {
        // socket 未初始化！
    }

    void send() {
        *socket = 1;           // 未定义行为
    }
};

void crash_case_2() {
    Connection conn(8080);
    conn.send();
}

// ❌ 崩溃：条件分支遗漏初始化
void crash_case_3(int flag) {
    int* p;
    if (flag > 0) {
        int x = 10;
        p = &x;
    }
    // flag <= 0 时 p 未初始化
    std::cout << *p << "\n";
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    // crash_case_3(0);
    std::cout << "uncomment a case to test\n";
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或静默损坏
 *
 * 难点: 行为不确定，有时碰巧"正常"，难以复现
 *
 * GCC/Clang 警告:
 *   -Wuninitialized  可以检测部分情况
 *
 * UBSan 输出:
 *   runtime error: load of null pointer (或其他非法值)
 *
 * 修复方案:
 *   1. 声明时立即初始化: int* p = nullptr;
 *   2. 开启 -Wuninitialized -Werror
 *   3. 使用智能指针 (默认构造为 nullptr)
 *   4. 使用 clang-tidy static analysis
 *   5. 成员初始化列表写完整
 */
