// 空指针解引用 — C++ 面试崩溃用例 #01
// 编译: g++ -g -o 01_null_pointer 01_null_pointer.cpp
// ASan: g++ -fsanitize=address -g -o 01_null_pointer 01_null_pointer.cpp

#include <iostream>

struct Widget {
    int value;
    void print() { std::cout << value << "\n"; }
};

// ❌ 崩溃：对 nullptr 解引用
void crash_case_1() {
    Widget* p = nullptr;
    p->print();          // SIGSEGV
}

// ❌ 崩溃：malloc 失败未检查
void crash_case_2() {
    char* buf = (char*)0;  // 模拟 malloc 返回 nullptr
    buf[0] = 'a';          // SIGSEGV
}

// ❌ 崩溃：函数返回 nullptr 未检查
Widget* find_widget(int id) {
    if (id < 0) return nullptr;
    static Widget w{42};
    return &w;
}

void crash_case_3() {
    Widget* w = find_widget(-1);
    std::cout << w->value << "\n";   // SIGSEGV
}

// ❌ 崩溃：多态调用时 this 为 nullptr
struct Base {
    virtual void foo() { std::cout << "Base\n"; }
};

void crash_case_4() {
    Base* b = nullptr;
    b->foo();             // 某些编译器下会崩溃（虚函数表指针解引用）
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
 * 崩溃信号: SIGSEGV (11)
 *
 * GDB 定位:
 *   gdb -ex run -ex bt ./01_null_pointer
 *   会停在 p->print() 这一行
 *
 * ASan 输出:
 *   runtime error: null pointer passed as argument 1
 *
 * 修复方案:
 *   1. 所有指针使用前检查 nullptr
 *   2. 优先使用智能指针 (unique_ptr/shared_ptr)
 *   3. 使用 C++20 std::assume 或 [[gsl::not_null]]
 *   4. 接口设计返回 optional 而非裸指针
 */
