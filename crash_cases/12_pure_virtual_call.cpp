// 纯虚函数调用 — C++ 面试崩溃用例 #12
// 编译: g++ -g -std=c++17 -o 12_pure_virtual_call 12_pure_virtual_call.cpp

#include <iostream>

// ❌ 崩溃：构造函数中调用虚函数
struct Base {
    Base() {
        init();               // 构造期间调用虚函数，调用的是 Base::init 而非 Derived::init
    }
    virtual void init() = 0;  // 纯虚函数 → 崩溃
};

struct Derived : Base {
    Derived() : Base() {}
    void init() override { std::cout << "Derived::init\n"; }
};

void crash_case_1() {
    Derived d;               // 构造 Base 时调用纯虚函数 init() → crash
}

// ❌ 崩溃：析构函数中调用纯虚函数
struct Animal {
    virtual ~Animal() { cleanup(); }    // 析构时调用虚函数
    virtual void cleanup() = 0;
};

struct Dog : Animal {
    void cleanup() override { std::cout << "Dog cleanup\n"; }
};

void crash_case_2() {
    Dog d;                    // 析构 Animal 时调用纯虚函数 cleanup() → crash
}

// ❌ 崩溃：通过未完全构造的对象调用虚函数
struct Shape {
    Shape() { draw(); }
    virtual void draw() = 0;
};

void crash_case_3() {
    struct Circle : Shape {
        void draw() override { std::cout << "circle\n"; }
    };
    Circle c;                // 崩溃
}

// ✅ 修复：用两阶段初始化
class SafeBase {
public:
    SafeBase() = default;
    virtual void init() = 0;
    void initialize() { init(); }   // 构造完成后手动调用
};

class SafeDerived : public SafeBase {
public:
    void init() override { std::cout << "SafeDerived::init\n"; }
};

void fix_case() {
    SafeDerived d;
    d.initialize();                // 此时对象已完全构造
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    // crash_case_3();
    fix_case();
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGABRT (6) — pure virtual method called
 *
 * 原因:
 *   C++ 对象构造/析构期间，虚函数表指向当前层级
 *   构造 Base 时，vptr 指向 Base 的 vtable，不会调用 Derived 的重写
 *   如果 Base 中是纯虚函数 → 调用 __cxa_pure_virtual → abort
 *
 * GDB 定位:
 *   bt 会看到 __cxa_pure_virtual → abort
 *
 * 修复方案:
 *   1. 构造函数中不调用虚函数
 *   2. 用两阶段初始化 (先构造，再 init())
 *   3. 用非虚函数 + CRTP (静态多态)
 *   4. 构造函数中调用非虚的私有实现
 *   5. 使用模板方法模式时注意调用时机
 */
