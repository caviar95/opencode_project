// 悬垂指针 / Use After Free — C++ 面试崩溃用例 #02
// 编译: g++ -g -o 02_dangling_pointer 02_dangling_pointer.cpp
// ASan: g++ -fsanitize=address -g -o 02_dangling_pointer 02_dangling_pointer.cpp

#include <iostream>
#include <cstring>

// ❌ 崩溃：使用已释放的内存
void crash_case_1() {
    int* p = new int(42);
    delete p;
    std::cout << *p << "\n";    // Use After Free，可能崩溃或输出垃圾值
}

// ❌ 崩溃：返回局部变量的地址
int* get_ptr() {
    int x = 100;
    return &x;                  // x 在函数返回后被销毁
}

void crash_case_2() {
    int* p = get_ptr();
    std::cout << *p << "\n";    // 未定义行为
}

// ❌ 崩溃：use-after-free 在对象上
struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

void crash_case_3() {
    Node* head = new Node(1);
    head->next = new Node(2);
    delete head;
    // 此时 head 是悬垂指针
    head->next = nullptr;       // Use After Free
}

// ❌ 崩溃：容器迭代器失效后使用
#include <vector>
void crash_case_4() {
    std::vector<int> v = {1, 2, 3};
    auto it = v.begin();
    v.push_back(4);             // 可能触发 rehash，it 失效
    std::cout << *it << "\n";   // 未定义行为
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
 * 崩溃信号: SIGSEGV (11) 或静默数据损坏
 *
 * GDB 定位:
 *   较难，因为崩溃点和 bug 点通常不在一处
 *   需要用 watchpoint: watch *0xADDR
 *
 * ASan 输出 (非常精准):
 *   ERROR: AddressSanitizer: heap-use-after-free
 *     READ of size 4 at 0xADDR
 *
 * 修复方案:
 *   1. 指针置空: delete p; p = nullptr;
 *   2. 使用 unique_ptr / shared_ptr 管理生命周期
 *   3. 返回值而非指针
 *   4. 遵循 RAII 原则
 *   5. 了解容器迭代器失效规则
 */
