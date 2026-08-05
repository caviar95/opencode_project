// 迭代器失效 — C++ 面试崩溃用例 #11
// 编译: g++ -g -std=c++17 -o 11_iterator_invalidate 11_iterator_invalidate.cpp
// ASan: g++ -fsanitize=address -g -std=c++17 -o 11_iterator_invalidate 11_iterator_invalidate.cpp

#include <iostream>
#include <vector>
#include <list>
#include <map>

// ❌ 崩溃：vector insert/erase 后迭代器失效
void crash_case_1() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.begin();
    v.insert(v.begin(), 0);       // insert 可能使所有迭代器失效
    std::cout << *it << "\n";     // 未定义行为
}

// ❌ 崩溃：遍历中 erase 未更新迭代器
void crash_case_2() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (*it % 2 == 0) {
            v.erase(it);          // erase 后 it 失效，++it 是 UB
        }
    }
}

// ❌ 崩溃：map 遍历时 erase (不同于 vector，map erase 只使被删元素的迭代器失效)
void crash_case_3() {
    std::map<int, std::string> m = {{1, "a"}, {2, "b"}, {3, "c"}};
    for (auto it = m.begin(); it != m.end(); ++it) {
        if (it->first == 2) {
            m.erase(it);          // it 失效
        }
        // 即使没删到，it 在某些实现下可能有问题
    }
}

// ✅ 修复：vector 遍历时正确 erase
void fix_case_1() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0) {
            it = v.erase(it);     // erase 返回下一个有效迭代器
        } else {
            ++it;
        }
    }
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

// ✅ 修复：map 遍历时正确 erase (C++11 起 erase 返回下一个迭代器)
void fix_case_2() {
    std::map<int, std::string> m = {{1, "a"}, {2, "b"}, {3, "c"}};
    for (auto it = m.begin(); it != m.end(); ) {
        if (it->first == 2) {
            it = m.erase(it);
        } else {
            ++it;
        }
    }
    for (auto& [k, v] : m) std::cout << k << ":" << v << " ";
    std::cout << "\n";
}

// ✅ 修复：用 erase-remove idiom (vector 最佳实践)
void fix_case_3() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    v.erase(std::remove_if(v.begin(), v.end(),
                           [](int x) { return x % 2 == 0; }),
            v.end());
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    // 取消注释逐一测试 crash case
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
 * 容器迭代器失效规则 (高频考点):
 *
 * vector:
 *   insert — 插入点及之后全部失效，扩容则全部失效
 *   erase  — 被删元素及之后全部失效
 *
 * deque:
 *   insert/erase — 所有迭代器全部失效
 *
 * list / forward_list:
 *   insert — 不使任何迭代器失效
 *   erase  — 仅使被删元素的迭代器失效
 *
 * set / map (关联容器):
 *   insert — 不使任何迭代器失效
 *   erase  — 仅使被删元素的迭代器失效
 *
 * unordered_set / unordered_map:
 *   insert — 可能 rehash，全部失效
 *   erase  — 仅使被删元素的迭代器失效
 *
 * 修复方案:
 *   1. 用 erase 返回值更新迭代器
 *   2. 用 erase-remove idiom
 *   3. 遍历时先保存下一个迭代器: auto next = std::next(it);
 */
