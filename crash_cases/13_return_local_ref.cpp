// 返回局部变量引用/指针 — C++ 面试崩溃用例 #13
// 编译: g++ -g -std=c++17 -o 13_return_local_ref 13_return_local_ref.cpp

#include <iostream>
#include <string>

// ❌ 崩溃：返回局部变量的引用
std::string& get_name() {
    std::string name = "Alice";
    return name;                    // name 在函数返回时被销毁
}

void crash_case_1() {
    std::string& ref = get_name();
    std::cout << ref << "\n";       // 未定义行为
}

// ❌ 崩溃：返回局部变量的指针
int* get_value() {
    int x = 42;
    return &x;                      // x 已被销毁
}

void crash_case_2() {
    int* p = get_value();
    std::cout << *p << "\n";        // 未定义行为
}

// ❌ 崩溃：返回临时对象的引用
const std::string& make_upper(const std::string& s) {
    std::string result = s;
    for (auto& c : result) c = toupper(c);
    return result;                  // result 是局部变量
}

void crash_case_3() {
    const std::string& upper = make_upper("hello");
    std::cout << upper << "\n";     // 未定义行为
}

// ✅ 修复：返回值 (现代 C++ 有 RVO/NRVO 优化)
std::string get_name_fixed() {
    std::string name = "Alice";
    return name;                    // RVO/NRVO 优化，无额外拷贝
}

void fix_case_1() {
    std::string name = get_name_fixed();
    std::cout << name << "\n";
}

// ✅ 修复：返回 static (适合单例/常量)
const std::string& get_constant() {
    static const std::string msg = "Hello, World!";
    return msg;
}

void fix_case_2() {
    const std::string& ref = get_constant();
    std::cout << ref << "\n";
}

// ✅ 修复：通过参数传出
void get_value_out(int& out) {
    out = 42;
}

void fix_case_3() {
    int value;
    get_value_out(value);
    std::cout << value << "\n";
}

int main() {
    // 取消注释逐一测试
    // crash_case_1();
    // crash_case_2();
    // crash_case_3();

    fix_case_1();
    fix_case_2();
    fix_case_3();
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或静默数据损坏
 *
 * 编译器警告:
 *   -Wreturn-local-addr   GCC/Clang 可检测并警告
 *
 * 关键区分:
 *   返回局部变量值   ✅ OK (有 RVO 优化)
 *   返回局部变量引用 ❌ 悬垂引用
 *   返回局部变量指针 ❌ 悬垂指针
 *   返回 static 引用 ✅ OK (生命周期到程序结束)
 *   返回成员引用     ⚠️ 需确保对象存活
 *   返回临时对象     ❌ 但 const T& 绑定可延长生命周期 (仅限直接绑定)
 *
 * 修复方案:
 *   1. 返回值而非引用 (现代 C++ 零成本)
 *   2. 用 static 延长生命周期 (注意线程安全)
 *   3. 通过输出参数传出
 *   4. 返回智能指针管理动态对象
 *   5. 开启 -Wreturn-local-addr -Werror
 */
