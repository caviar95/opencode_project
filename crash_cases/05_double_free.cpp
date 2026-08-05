// 重复释放 (Double Free) — C++ 面试崩溃用例 #05
// 编译: g++ -g -o 05_double_free 05_double_free.cpp
// ASan: g++ -fsanitize=address -g -o 05_double_free 05_double_free.cpp

#include <iostream>

// ❌ 崩溃：显式重复 delete
void crash_case_1() {
    int* p = new int(42);
    delete p;
    delete p;                // double free，通常立即崩溃
}

// ❌ 崩溃：浅拷贝导致重复释放
struct Buffer {
    char* data;
    size_t size;

    Buffer(size_t s) : size(s) { data = new char[s]; }

    // 缺少拷贝构造函数和拷贝赋值 — 编译器生成的版本做浅拷贝
    ~Buffer() { delete[] data; }
};

void crash_case_2() {
    Buffer a(100);
    Buffer b = a;           // 浅拷贝，b.data == a.data
}                           // a 和 b 析构时各自 delete[] data

// ❌ 崩溃：异常安全导致 double free
void crash_case_3() {
    int* p = new int(42);
    try {
        throw std::runtime_error("error");
    } catch (...) {
        delete p;
    }
    delete p;                // 异常捕获后又释放了一次
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
 * 崩溃信号: SIGABRT (6) — glibc 检测到 double free
 *
 * GDB 定位:
 *   gdb -ex run -ex bt ./05_double_free
 *   会停在 malloc_printerr → __libc_message → abort
 *
 * ASan 输出:
 *   ERROR: AddressSanitizer: attempting double-free
 *
 * Valgrind 输出:
 *   Invalid free() / delete / delete[] / realloc()
 *
 * 修复方案:
 *   1. 遵循 Rule of Three/Five/Zero
 *   2. 使用 unique_ptr (不可拷贝) 防止意外
 *   3. 深拷贝 or 禁用拷贝 = delete
 *   4. delete 后置 nullptr
 *   5. 实现移动语义避免不必要的拷贝
 */
