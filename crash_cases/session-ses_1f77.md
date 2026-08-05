# New session - 2026-05-08T17:07:53.556Z

**Session ID:** ses_1f770a3abffetV2fKJP4ClkNeQ
**Created:** 5/9/2026, 1:07:53 AM
**Updated:** 5/9/2026, 1:13:27 AM

---

## User

创建目录，用于生成常见的进程崩溃用例，以及给出定位思路手段和解决问题的方案，我的目标是应对c++面试

---

## Assistant (Build · Big Pickle · 9.6s)

_Thinking:_

The user wants me to create a directory structure with common C++ process crash use cases, along with debugging approaches and solutions. This is specifically for preparing for C++ interviews. Let me create a comprehensive set of examples covering common crash scenarios.

I'll create:
1. A directory structure
2. Multiple crash case examples with code
3. Debugging/positioning methods
4. Solutions for each case

Common C++ crash scenarios:
1. Null pointer dereference
2. Dangling pointer / Use after free
3. Buffer overflow
4. Stack overflow
5. Double free
6. Uninitialized pointer
7. Division by zero
8. Race condition / data race
9. Deadlock
10. Out of bounds array access
11. Memory leak (not crash but common interview topic)
12. Undefined behavior with iterators

Let me create this structure.

**Tool: bash**

**Input:**
```json
{
  "command": "mkdir -p /Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases",
  "description": "Create directory for crash cases"
}
```

---

## Assistant (Build · Big Pickle · 19.7s)

_Thinking:_

Now I'll create all the files in parallel. Let me prepare:
1. README.md - overview and debugging guide
2. Multiple crash case files with code and solutions

**Tool: write**

**Input:**
```json
{
  "content": "# C++ 进程崩溃用例 & 面试指南\n\n## 目录结构\n\n| 文件 | 崩溃类型 |\n|------|----------|\n| `01_null_pointer.cpp` | 空指针解引用 |\n| `02_dangling_pointer.cpp` | 悬垂指针 / Use After Free |\n| `03_buffer_overflow.cpp` | 缓冲区溢出 |\n| `04_stack_overflow.cpp` | 栈溢出 |\n| `05_double_free.cpp` | 重复释放 |\n| `06_uninitialized_ptr.cpp` | 未初始化指针 |\n| `07_division_by_zero.cpp` | 除零错误 |\n| `08_out_of_bounds.cpp` | 数组越界 |\n| `09_data_race.cpp` | 数据竞争 |\n| `10_deadlock.cpp` | 死锁 |\n| `11_iterator_invalidate.cpp` | 迭代器失效 |\n| `12_pure_virtual_call.cpp` | 纯虚函数调用 |\n| `13_return_local_ref.cpp` | 返回局部变量引用 |\n| `14_static_destruction_order.cpp` | 静态对象析构顺序 |\n\n## 定位手段（面试高频）\n\n### 1. GDB 调试\n```bash\ngdb -ex run -ex bt ./program          # 运行并打印回溯\ngdb -ex \"set pagination off\" -ex run -ex \"info registers\" -ex bt ./program\ngdb ./program core.xxx                # 分析 core dump\n```\n\n### 2. Core Dump 分析\n```bash\nulimit -c unlimited                   # 开启 core dump\necho \"/tmp/core.%e.%p\" > /proc/sys/kernel/core_pattern\ngdb ./program /tmp/core.xxx           # 加载分析\n```\n\n### 3. AddressSanitizer (ASan)\n```bash\ng++ -fsanitize=address -g -o prog prog.cpp    # 编译\n./prog                                         # 运行即报\n```\n能检测：内存越界、Use-After-Free、Double-Free、内存泄漏\n\n### 4. UndefinedBehaviorSanitizer (UBSan)\n```bash\ng++ -fsanitize=undefined -g -o prog prog.cpp\n```\n能检测：未定义行为（空指针、对齐错误、整型溢出等）\n\n### 5. ThreadSanitizer (TSan)\n```bash\ng++ -fsanitize=thread -g -o prog prog.cpp\n```\n能检测：数据竞争\n\n### 6. Valgrind\n```bash\nvalgrind --leak-check=full --track-origins=yes ./program\n```\n\n### 7. strace / ltrace\n```bash\nstrace -f ./program       # 跟踪系统调用\nltrace ./program          # 跟踪库函数调用\n```\n\n### 8. pstack / gdb attach\n```bash\ngdb -p <pid> -ex \"thread apply all bt\" -ex quit    # 看所有线程栈\npstack <pid>\n```\n\n## 面试答题模板\n\n当面试官问 **\"进程崩溃怎么排查？\"** 按以下顺序回答：\n\n1. **看日志** — 应用日志、系统日志（`dmesg` 看 OOM/segfault）\n2. **看 Core Dump** — `gdb` 加载 core，`bt` 看调用栈\n3. **复现** — 用 ASan/UBSan 编译，快速定位\n4. **多线程问题** — 用 TSan、加日志、看线程状态\n5. **生产环境** — `gdb attach`、`strace`、检查资源（内存/FD）\n6. **预防** — 代码审查、静态分析（clang-tidy）、CI 集成 Sanitizer\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/README.md"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 14.4s)

_Thinking:_

Now let me create all the crash case files in parallel.

**Tool: write**

**Input:**
```json
{
  "content": "// 空指针解引用 — C++ 面试崩溃用例 #01\n// 编译: g++ -g -o 01_null_pointer 01_null_pointer.cpp\n// ASan: g++ -fsanitize=address -g -o 01_null_pointer 01_null_pointer.cpp\n\n#include <iostream>\n\nstruct Widget {\n    int value;\n    void print() { std::cout << value << \"\\n\"; }\n};\n\n// ❌ 崩溃：对 nullptr 解引用\nvoid crash_case_1() {\n    Widget* p = nullptr;\n    p->print();          // SIGSEGV\n}\n\n// ❌ 崩溃：malloc 失败未检查\nvoid crash_case_2() {\n    char* buf = (char*)0;  // 模拟 malloc 返回 nullptr\n    buf[0] = 'a';          // SIGSEGV\n}\n\n// ❌ 崩溃：函数返回 nullptr 未检查\nWidget* find_widget(int id) {\n    if (id < 0) return nullptr;\n    static Widget w{42};\n    return &w;\n}\n\nvoid crash_case_3() {\n    Widget* w = find_widget(-1);\n    std::cout << w->value << \"\\n\";   // SIGSEGV\n}\n\n// ❌ 崩溃：多态调用时 this 为 nullptr\nstruct Base {\n    virtual void foo() { std::cout << \"Base\\n\"; }\n};\n\nvoid crash_case_4() {\n    Base* b = nullptr;\n    b->foo();             // 某些编译器下会崩溃（虚函数表指针解引用）\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    // crash_case_4();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11)\n *\n * GDB 定位:\n *   gdb -ex run -ex bt ./01_null_pointer\n *   会停在 p->print() 这一行\n *\n * ASan 输出:\n *   runtime error: null pointer passed as argument 1\n *\n * 修复方案:\n *   1. 所有指针使用前检查 nullptr\n *   2. 优先使用智能指针 (unique_ptr/shared_ptr)\n *   3. 使用 C++20 std::assume 或 [[gsl::not_null]]\n *   4. 接口设计返回 optional 而非裸指针\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/01_null_pointer.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 16.3s)

_Thinking:_

Good, now let me create the rest of the files in parallel.

**Tool: write**

**Input:**
```json
{
  "content": "// 悬垂指针 / Use After Free — C++ 面试崩溃用例 #02\n// 编译: g++ -g -o 02_dangling_pointer 02_dangling_pointer.cpp\n// ASan: g++ -fsanitize=address -g -o 02_dangling_pointer 02_dangling_pointer.cpp\n\n#include <iostream>\n#include <cstring>\n\n// ❌ 崩溃：使用已释放的内存\nvoid crash_case_1() {\n    int* p = new int(42);\n    delete p;\n    std::cout << *p << \"\\n\";    // Use After Free，可能崩溃或输出垃圾值\n}\n\n// ❌ 崩溃：返回局部变量的地址\nint* get_ptr() {\n    int x = 100;\n    return &x;                  // x 在函数返回后被销毁\n}\n\nvoid crash_case_2() {\n    int* p = get_ptr();\n    std::cout << *p << \"\\n\";    // 未定义行为\n}\n\n// ❌ 崩溃：use-after-free 在对象上\nstruct Node {\n    int data;\n    Node* next;\n    Node(int d) : data(d), next(nullptr) {}\n};\n\nvoid crash_case_3() {\n    Node* head = new Node(1);\n    head->next = new Node(2);\n    delete head;\n    // 此时 head 是悬垂指针\n    head->next = nullptr;       // Use After Free\n}\n\n// ❌ 崩溃：容器迭代器失效后使用\n#include <vector>\nvoid crash_case_4() {\n    std::vector<int> v = {1, 2, 3};\n    auto it = v.begin();\n    v.push_back(4);             // 可能触发 rehash，it 失效\n    std::cout << *it << \"\\n\";   // 未定义行为\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    // crash_case_4();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或静默数据损坏\n *\n * GDB 定位:\n *   较难，因为崩溃点和 bug 点通常不在一处\n *   需要用 watchpoint: watch *0xADDR\n *\n * ASan 输出 (非常精准):\n *   ERROR: AddressSanitizer: heap-use-after-free\n *     READ of size 4 at 0xADDR\n *\n * 修复方案:\n *   1. 指针置空: delete p; p = nullptr;\n *   2. 使用 unique_ptr / shared_ptr 管理生命周期\n *   3. 返回值而非指针\n *   4. 遵循 RAII 原则\n *   5. 了解容器迭代器失效规则\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/02_dangling_pointer.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 15.0s)

_Thinking:_

Continuing to create the remaining files in parallel batches.

**Tool: write**

**Input:**
```json
{
  "content": "// 缓冲区溢出 — C++ 面试崩溃用例 #03\n// 编译: g++ -g -o 03_buffer_overflow 03_buffer_overflow.cpp\n// ASan: g++ -fsanitize=address -g -o 03_buffer_overflow 03_buffer_overflow.cpp\n\n#include <iostream>\n#include <cstring>\n\n// ❌ 崩溃：栈缓冲区溢出\nvoid crash_case_1() {\n    char buf[4];\n    strcpy(buf, \"this is a very long string\");  // 溢出，覆盖栈上其他数据\n    std::cout << buf << \"\\n\";\n}\n\n// ❌ 崩溃：堆缓冲区溢出\nvoid crash_case_2() {\n    int* arr = new int[5];\n    arr[10] = 42;              // 越界写入，破坏堆元数据\n    delete[] arr;              // 可能在 delete 时崩溃\n}\n\n// ❌ 崩溃：off-by-one 经典错误\nvoid crash_case_3() {\n    char buf[5];\n    for (int i = 0; i <= 5; i++) {  // 应该是 < 5\n        buf[i] = 'a';\n    }\n    buf[5] = '\\0';               // off-by-one 写入\n}\n\n// ❌ 崩溃：格式化字符串漏洞 + 溢出\nvoid crash_case_4() {\n    char dst[8];\n    char src[] = \"hello world overflow!\";\n    memcpy(dst, src, strlen(src));   // 目标缓冲区太小\n    std::cout << dst << \"\\n\";\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    // crash_case_4();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或 SIGABRT (6)\n *\n * GDB 定位:\n *   栈溢出可能覆盖返回地址，导致跳到非法地址\n *   bt 会看到奇怪的函数名或 ??\n *\n * ASan 输出:\n *   ERROR: AddressSanitizer: heap-buffer-overflow\n *   ERROR: AddressSanitizer: stack-buffer-overflow\n *\n * 修复方案:\n *   1. 用 std::string / std::vector 替代裸数组\n *   2. 用 strncpy / snprintf 替代 strcpy / sprintf\n *   3. 用 std::array / std::span (C++20) 做边界检查\n *   4. 编译加 -fstack-protector-all (栈保护)\n *   5. CI 中跑 ASan\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/03_buffer_overflow.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 15.0s)

_Thinking:_

Let me continue creating the remaining files in parallel to speed things up.

**Tool: write**

**Input:**
```json
{
  "content": "// 栈溢出 — C++ 面试崩溃用例 #04\n// 编译: g++ -g -o 04_stack_overflow 04_stack_overflow.cpp\n\n#include <iostream>\n\n// ❌ 崩溃：无限递归\nvoid crash_case_1(int n) {\n    std::cout << n << \"\\n\";\n    crash_case_1(n + 1);       // 无终止条件，栈耗尽\n}\n\n// ❌ 崩溃：超大栈分配\nvoid crash_case_2() {\n    char buf[10 * 1024 * 1024];  // 10MB，超过默认栈大小 (通常 8MB)\n    buf[0] = 'a';\n}\n\n// ❌ 崩溃：递归深度过大\nint fibonacci(int n) {\n    if (n <= 1) return n;\n    return fibonacci(n - 1) + fibonacci(n - 2);  // 指数级栈增长\n}\n\nvoid crash_case_3() {\n    std::cout << fibonacci(100) << \"\\n\";   // 栈溢出\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1(0);\n    // crash_case_2();\n    // crash_case_3();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或 SIGBUS\n *\n * GDB 定位:\n *   gdb -ex run -ex bt ./04_stack_overflow\n *   bt 会看到同一函数重复出现数千次\n *\n * 特征:\n *   崩溃地址通常在栈底附近 (如 0x7fffxxxxx 接近栈边界)\n *\n * 修复方案:\n *   1. 递归改迭代 (尾递归优化并非所有编译器都支持)\n *   2. 大对象分配到堆 (用 vector/new)\n *   3. 限制递归深度\n *   4. ulimit -s 查看/调整栈大小\n *   5. 用 BFS 替代 DFS (减少栈深度)\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/04_stack_overflow.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 13.6s)

_Thinking:_

Let me create more files in parallel.

**Tool: write**

**Input:**
```json
{
  "content": "// 重复释放 (Double Free) — C++ 面试崩溃用例 #05\n// 编译: g++ -g -o 05_double_free 05_double_free.cpp\n// ASan: g++ -fsanitize=address -g -o 05_double_free 05_double_free.cpp\n\n#include <iostream>\n\n// ❌ 崩溃：显式重复 delete\nvoid crash_case_1() {\n    int* p = new int(42);\n    delete p;\n    delete p;                // double free，通常立即崩溃\n}\n\n// ❌ 崩溃：浅拷贝导致重复释放\nstruct Buffer {\n    char* data;\n    size_t size;\n\n    Buffer(size_t s) : size(s) { data = new char[s]; }\n\n    // 缺少拷贝构造函数和拷贝赋值 — 编译器生成的版本做浅拷贝\n    ~Buffer() { delete[] data; }\n};\n\nvoid crash_case_2() {\n    Buffer a(100);\n    Buffer b = a;           // 浅拷贝，b.data == a.data\n}                           // a 和 b 析构时各自 delete[] data\n\n// ❌ 崩溃：异常安全导致 double free\nvoid crash_case_3() {\n    int* p = new int(42);\n    try {\n        throw std::runtime_error(\"error\");\n    } catch (...) {\n        delete p;\n    }\n    delete p;                // 异常捕获后又释放了一次\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGABRT (6) — glibc 检测到 double free\n *\n * GDB 定位:\n *   gdb -ex run -ex bt ./05_double_free\n *   会停在 malloc_printerr → __libc_message → abort\n *\n * ASan 输出:\n *   ERROR: AddressSanitizer: attempting double-free\n *\n * Valgrind 输出:\n *   Invalid free() / delete / delete[] / realloc()\n *\n * 修复方案:\n *   1. 遵循 Rule of Three/Five/Zero\n *   2. 使用 unique_ptr (不可拷贝) 防止意外\n *   3. 深拷贝 or 禁用拷贝 = delete\n *   4. delete 后置 nullptr\n *   5. 实现移动语义避免不必要的拷贝\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/05_double_free.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 14.6s)

_Thinking:_

Continuing to create the remaining files.

**Tool: write**

**Input:**
```json
{
  "content": "// 未初始化指针 — C++ 面试崩溃用例 #06\n// 编译: g++ -g -o 06_uninitialized_ptr 06_uninitialized_ptr.cpp\n// UBSan: g++ -fsanitize=undefined -g -o 06_uninitialized_ptr 06_uninitialized_ptr.cpp\n\n#include <iostream>\n\n// ❌ 崩溃：未初始化的局部指针\nvoid crash_case_1() {\n    int* p;                    // 值是随机的（栈上残留）\n    *p = 42;                   // 可能写到一个非法地址\n}\n\n// ❌ 崩溃：未初始化的成员指针\nstruct Connection {\n    int* socket;\n    int port;\n\n    Connection(int p) : port(p) {\n        // socket 未初始化！\n    }\n\n    void send() {\n        *socket = 1;           // 未定义行为\n    }\n};\n\nvoid crash_case_2() {\n    Connection conn(8080);\n    conn.send();\n}\n\n// ❌ 崩溃：条件分支遗漏初始化\nvoid crash_case_3(int flag) {\n    int* p;\n    if (flag > 0) {\n        int x = 10;\n        p = &x;\n    }\n    // flag <= 0 时 p 未初始化\n    std::cout << *p << \"\\n\";\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3(0);\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或静默损坏\n *\n * 难点: 行为不确定，有时碰巧\"正常\"，难以复现\n *\n * GCC/Clang 警告:\n *   -Wuninitialized  可以检测部分情况\n *\n * UBSan 输出:\n *   runtime error: load of null pointer (或其他非法值)\n *\n * 修复方案:\n *   1. 声明时立即初始化: int* p = nullptr;\n *   2. 开启 -Wuninitialized -Werror\n *   3. 使用智能指针 (默认构造为 nullptr)\n *   4. 使用 clang-tidy static analysis\n *   5. 成员初始化列表写完整\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/06_uninitialized_ptr.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 15.2s)

_Thinking:_

Let me create the remaining files in a batch.

**Tool: write**

**Input:**
```json
{
  "content": "// 除零错误 — C++ 面试崩溃用例 #07\n// 编译: g++ -g -o 07_division_by_zero 07_division_by_zero.cpp\n\n#include <iostream>\n\n// ❌ 崩溃：整数除零\nvoid crash_case_1() {\n    int a = 10;\n    int b = 0;\n    std::cout << a / b << \"\\n\";   // SIGFPE\n}\n\n// ❌ 崩溃：取模除零\nvoid crash_case_2() {\n    int x = 10;\n    int y = 0;\n    std::cout << x % y << \"\\n\";   // SIGFPE\n}\n\n// ❌ 崩溃：间接除零 (参数传递)\nint compute_ratio(int total, int count) {\n    return total / count;          // count 可能为 0\n}\n\nvoid crash_case_3() {\n    std::cout << compute_ratio(100, 0) << \"\\n\";\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGFPE (8) — Floating Point Exception (包括整数除零)\n *\n * GDB 定位:\n *   gdb -ex run -ex bt ./07_division_by_zero\n *   会精确停在除法指令处\n *\n * 注意: 浮点数除零不会崩溃，结果是 inf/nan\n *   double x = 1.0 / 0.0;   // x == inf，不崩溃\n *\n * 修复方案:\n *   1. 除法前检查分母: if (b == 0) return 0;\n *   2. 使用 std::optional 表示可能无效的结果\n *   3. 断言: assert(b != 0);\n *   4. 代码审查 + 静态分析\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/07_division_by_zero.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 17.3s)

_Thinking:_

Let me create several more files in parallel to speed things up.

**Tool: write**

**Input:**
```json
{
  "content": "// 数组/容器越界 — C++ 面试崩溃用例 #08\n// 编译: g++ -g -o 08_out_of_bounds 08_out_of_bounds.cpp\n// ASan: g++ -fsanitize=address -g -o 08_out_of_bounds 08_out_of_bounds.cpp\n\n#include <iostream>\n#include <vector>\n#include <string>\n\n// ❌ 崩溃：C 风格数组越界\nvoid crash_case_1() {\n    int arr[5] = {1, 2, 3, 4, 5};\n    std::cout << arr[10] << \"\\n\";     // 越界读取\n    arr[10] = 99;                     // 越界写入\n}\n\n// ❌ 崩溃：vector 下标越界 (operator[] 不检查)\nvoid crash_case_2() {\n    std::vector<int> v = {1, 2, 3};\n    v[5] = 10;                        // 越界，静默损坏或崩溃\n    std::cout << v[5] << \"\\n\";\n}\n\n// ❌ 崩溃：string 越界访问\nvoid crash_case_3() {\n    std::string s = \"hello\";\n    char c = s[100];                  // 越界\n    std::cout << c << \"\\n\";\n}\n\n// ❌ 崩溃：at() 会抛异常但常被忽略\nvoid crash_case_4() {\n    std::vector<int> v = {1, 2, 3};\n    try {\n        std::cout << v.at(10) << \"\\n\";   // 抛 std::out_of_range\n    } catch (const std::out_of_range& e) {\n        std::cout << \"caught: \" << e.what() << \"\\n\";\n    }\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    // crash_case_4();\n    std::cout << \"uncomment a case to test\\n\";\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或 std::out_of_range 异常\n *\n * GDB 定位:\n *   ASan 最精准，直接报越界位置和大小\n *\n * ASan 输出:\n *   ERROR: AddressSanitizer: heap-buffer-overflow on address 0xADDR\n *\n * operator[] vs at():\n *   v[i]   — 不检查边界，性能高，越界是 UB\n *   v.at(i) — 检查边界，越界抛 std::out_of_range\n *\n * 修复方案:\n *   1. 用 at() 替代 operator[] (调试阶段)\n *   2. 用迭代器 + begin()/end() 遍历\n *   3. 用 range-based for 循环\n *   4. 用 std::span (C++20) 做安全视图\n *   5. CI 中跑 ASan\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/08_out_of_bounds.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 20.3s)

_Thinking:_

Let me create the remaining files in parallel.

**Tool: write**

**Input:**
```json
{
  "content": "// 数据竞争 (Data Race) — C++ 面试崩溃用例 #09\n// 编译: g++ -g -std=c++17 -pthread -o 09_data_race 09_data_race.cpp\n// TSan: g++ -fsanitize=thread -g -std=c++17 -pthread -o 09_data_race 09_data_race.cpp\n\n#include <iostream>\n#include <thread>\n#include <vector>\n\n// ❌ 崩溃：多线程同时写共享变量 (无保护)\nint shared_counter = 0;\n\nvoid increment(int id, int iterations) {\n    for (int i = 0; i < iterations; i++) {\n        shared_counter++;          // 非原子操作，数据竞争\n    }\n}\n\nvoid crash_case_1() {\n    std::vector<std::thread> threads;\n    for (int i = 0; i < 10; i++) {\n        threads.emplace_back(increment, i, 10000);\n    }\n    for (auto& t : threads) t.join();\n\n    // 结果几乎不会是 100000\n    std::cout << \"counter = \" << shared_counter << \"\\n\";\n}\n\n// ❌ 崩溃：多线程同时修改容器\nvoid crash_case_2() {\n    std::vector<int> v;\n\n    auto writer = [&]() {\n        for (int i = 0; i < 1000; i++) {\n            v.push_back(i);        // 和 reader 并发\n        }\n    };\n\n    auto reader = [&]() {\n        for (int i = 0; i < 1000; i++) {\n            if (!v.empty()) {\n                std::cout << v.back() << \"\\n\";  // 可能读到无效数据\n            }\n        }\n    };\n\n    std::thread t1(writer);\n    std::thread t2(reader);\n    t1.join();\n    t2.join();\n}\n\n// ✅ 修复：用 mutex 保护\n#include <mutex>\nstd::mutex mtx;\nint safe_counter = 0;\n\nvoid safe_increment(int iterations) {\n    for (int i = 0; i < iterations; i++) {\n        std::lock_guard<std::mutex> lock(mtx);\n        safe_counter++;\n    }\n}\n\nvoid fix_case() {\n    std::vector<std::thread> threads;\n    for (int i = 0; i < 10; i++) {\n        threads.emplace_back(safe_increment, 10000);\n    }\n    for (auto& t : threads) t.join();\n    std::cout << \"safe_counter = \" << safe_counter << \"\\n\";  // 正确: 100000\n}\n\n// ✅ 修复：用 atomic\n#include <atomic>\nstd::atomic<int> atomic_counter{0};\n\nvoid atomic_increment(int iterations) {\n    for (int i = 0; i < iterations; i++) {\n        atomic_counter++;\n    }\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    fix_case();\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或静默数据损坏\n *\n * 特征:\n *   难以复现，只在特定调度下出现 (heisenbug)\n *   加了 printf 可能就不出现了\n *\n * TSan 输出:\n *   WARNING: ThreadSanitizer: data race\n *\n * GDB 定位:\n *   多线程 gdb attach，看各线程状态\n *   thread apply all bt\n *\n * 修复方案:\n *   1. 用 std::mutex + lock_guard 保护共享数据\n *   2. 用 std::atomic (简单类型)\n *   3. 避免共享，用 thread-local 或消息传递\n *   4. 用 RAII 锁管理，避免忘记 unlock\n *   5. 定期跑 TSan\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/09_data_race.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 24.3s)

_Thinking:_

Let me continue creating the remaining files.

**Tool: write**

**Input:**
```json
{
  "content": "// 死锁 (Deadlock) — C++ 面试崩溃用例 #10\n// 编译: g++ -g -std=c++17 -pthread -o 10_deadlock 10_deadlock.cpp\n\n#include <iostream>\n#include <thread>\n#include <mutex>\n#include <chrono>\n\nstd::mutex mtx_a;\nstd::mutex mtx_b;\n\n// ❌ 死锁：两个线程以不同顺序获取锁\nvoid thread_1() {\n    mtx_a.lock();\n    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 增大死锁概率\n    mtx_b.lock();                    // 等 mtx_b，但 thread_2 持有它\n    std::cout << \"thread_1 got both locks\\n\";\n    mtx_b.unlock();\n    mtx_a.unlock();\n}\n\nvoid thread_2() {\n    mtx_b.lock();\n    std::this_thread::sleep_for(std::chrono::milliseconds(10));\n    mtx_a.lock();                    // 等 mtx_a，但 thread_1 持有它\n    std::cout << \"thread_2 got both locks\\n\";\n    mtx_a.unlock();\n    mtx_b.unlock();\n}\n\nvoid crash_case_1() {\n    std::thread t1(thread_1);\n    std::thread t2(thread_2);\n    t1.join();\n    t2.join();                       // 永远不会到这里\n}\n\n// ❌ 死锁：同一线程递归锁 (mutex 不支持递归)\nstd::mutex non_recursive;\n\nvoid inner() {\n    non_recursive.lock();\n    std::cout << \"inner\\n\";\n    non_recursive.unlock();\n}\n\nvoid outer() {\n    non_recursive.lock();\n    inner();                         // inner 再次 lock 同一个 mutex → 死锁\n    non_recursive.unlock();\n}\n\nvoid crash_case_2() {\n    outer();\n}\n\n// ✅ 修复：用 std::lock 同时获取多个锁\nvoid safe_thread_1() {\n    std::lock(mtx_a, mtx_b);\n    std::lock_guard<std::mutex> lock_a(mtx_a, std::adopt_lock);\n    std::lock_guard<std::mutex> lock_b(mtx_b, std::adopt_lock);\n    std::cout << \"safe_thread_1 got both locks\\n\";\n}\n\nvoid safe_thread_2() {\n    std::lock(mtx_b, mtx_a);         // 顺序无关，std::lock 避免死锁\n    std::lock_guard<std::mutex> lock_b(mtx_b, std::adopt_lock);\n    std::lock_guard<std::mutex> lock_a(mtx_a, std::adopt_lock);\n    std::cout << \"safe_thread_2 got both locks\\n\";\n}\n\nvoid fix_case() {\n    std::thread t1(safe_thread_1);\n    std::thread t2(safe_thread_2);\n    t1.join();\n    t2.join();\n}\n\n// ✅ 修复：用 std::scoped_lock (C++17 推荐)\nvoid fix_case_cpp17() {\n    auto worker = [](const char* name) {\n        std::scoped_lock lock(mtx_a, mtx_b);  // 自动避免死锁\n        std::cout << name << \" got both locks\\n\";\n    };\n    std::thread t1(worker, \"t1\");\n    std::thread t2(worker, \"t2\");\n    t1.join();\n    t2.join();\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();              // 会挂起\n    // crash_case_2();\n    // fix_case();\n    fix_case_cpp17();\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 表现: 进程不崩溃但卡死，CPU 可能很低 (线程在等待)\n *\n * GDB 定位:\n *   gdb -p <pid>\n *   thread apply all bt               # 看所有线程栈\n *   info mutex (gdb 9.2+)             # 看锁状态\n *\n * 死锁四条件 (Coffman):\n *   1. 互斥        — 资源不可共享\n *   2. 持有并等待   — 持有一个锁等另一个\n *   3. 不可抢占     — 锁不能被强制释放\n *   4. 循环等待     — A→B→A 的依赖环\n *\n * 修复方案:\n *   1. 统一加锁顺序 (破坏循环等待)\n *   2. std::lock / std::scoped_lock 同时获取多个锁\n *   3. 用 std::recursive_mutex (但需谨慎)\n *   4. 用 std::try_lock + 超时 (破坏不可抢占)\n *   5. 减少锁粒度，缩小临界区\n *   6. 用无锁数据结构或 actor 模型\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/10_deadlock.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 25.8s)

_Thinking:_

Let me continue creating the remaining crash case files.

**Tool: write**

**Input:**
```json
{
  "content": "// 迭代器失效 — C++ 面试崩溃用例 #11\n// 编译: g++ -g -std=c++17 -o 11_iterator_invalidate 11_iterator_invalidate.cpp\n// ASan: g++ -fsanitize=address -g -std=c++17 -o 11_iterator_invalidate 11_iterator_invalidate.cpp\n\n#include <iostream>\n#include <vector>\n#include <list>\n#include <map>\n\n// ❌ 崩溃：vector insert/erase 后迭代器失效\nvoid crash_case_1() {\n    std::vector<int> v = {1, 2, 3, 4, 5};\n    auto it = v.begin();\n    v.insert(v.begin(), 0);       // insert 可能使所有迭代器失效\n    std::cout << *it << \"\\n\";     // 未定义行为\n}\n\n// ❌ 崩溃：遍历中 erase 未更新迭代器\nvoid crash_case_2() {\n    std::vector<int> v = {1, 2, 3, 4, 5};\n    for (auto it = v.begin(); it != v.end(); ++it) {\n        if (*it % 2 == 0) {\n            v.erase(it);          // erase 后 it 失效，++it 是 UB\n        }\n    }\n}\n\n// ❌ 崩溃：map 遍历时 erase (不同于 vector，map erase 只使被删元素的迭代器失效)\nvoid crash_case_3() {\n    std::map<int, std::string> m = {{1, \"a\"}, {2, \"b\"}, {3, \"c\"}};\n    for (auto it = m.begin(); it != m.end(); ++it) {\n        if (it->first == 2) {\n            m.erase(it);          // it 失效\n        }\n        // 即使没删到，it 在某些实现下可能有问题\n    }\n}\n\n// ✅ 修复：vector 遍历时正确 erase\nvoid fix_case_1() {\n    std::vector<int> v = {1, 2, 3, 4, 5};\n    for (auto it = v.begin(); it != v.end(); ) {\n        if (*it % 2 == 0) {\n            it = v.erase(it);     // erase 返回下一个有效迭代器\n        } else {\n            ++it;\n        }\n    }\n    for (int x : v) std::cout << x << \" \";\n    std::cout << \"\\n\";\n}\n\n// ✅ 修复：map 遍历时正确 erase (C++11 起 erase 返回下一个迭代器)\nvoid fix_case_2() {\n    std::map<int, std::string> m = {{1, \"a\"}, {2, \"b\"}, {3, \"c\"}};\n    for (auto it = m.begin(); it != m.end(); ) {\n        if (it->first == 2) {\n            it = m.erase(it);\n        } else {\n            ++it;\n        }\n    }\n    for (auto& [k, v] : m) std::cout << k << \":\" << v << \" \";\n    std::cout << \"\\n\";\n}\n\n// ✅ 修复：用 erase-remove idiom (vector 最佳实践)\nvoid fix_case_3() {\n    std::vector<int> v = {1, 2, 3, 4, 5};\n    v.erase(std::remove_if(v.begin(), v.end(),\n                           [](int x) { return x % 2 == 0; }),\n            v.end());\n    for (int x : v) std::cout << x << \" \";\n    std::cout << \"\\n\";\n}\n\nint main() {\n    // 取消注释逐一测试 crash case\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n\n    fix_case_1();\n    fix_case_2();\n    fix_case_3();\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 容器迭代器失效规则 (高频考点):\n *\n * vector:\n *   insert — 插入点及之后全部失效，扩容则全部失效\n *   erase  — 被删元素及之后全部失效\n *\n * deque:\n *   insert/erase — 所有迭代器全部失效\n *\n * list / forward_list:\n *   insert — 不使任何迭代器失效\n *   erase  — 仅使被删元素的迭代器失效\n *\n * set / map (关联容器):\n *   insert — 不使任何迭代器失效\n *   erase  — 仅使被删元素的迭代器失效\n *\n * unordered_set / unordered_map:\n *   insert — 可能 rehash，全部失效\n *   erase  — 仅使被删元素的迭代器失效\n *\n * 修复方案:\n *   1. 用 erase 返回值更新迭代器\n *   2. 用 erase-remove idiom\n *   3. 遍历时先保存下一个迭代器: auto next = std::next(it);\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/11_iterator_invalidate.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 18.5s)

_Thinking:_

Let me create the remaining crash case files.

**Tool: write**

**Input:**
```json
{
  "content": "// 纯虚函数调用 — C++ 面试崩溃用例 #12\n// 编译: g++ -g -std=c++17 -o 12_pure_virtual_call 12_pure_virtual_call.cpp\n\n#include <iostream>\n\n// ❌ 崩溃：构造函数中调用虚函数\nstruct Base {\n    Base() {\n        init();               // 构造期间调用虚函数，调用的是 Base::init 而非 Derived::init\n    }\n    virtual void init() = 0;  // 纯虚函数 → 崩溃\n};\n\nstruct Derived : Base {\n    Derived() : Base() {}\n    void init() override { std::cout << \"Derived::init\\n\"; }\n};\n\nvoid crash_case_1() {\n    Derived d;               // 构造 Base 时调用纯虚函数 init() → crash\n}\n\n// ❌ 崩溃：析构函数中调用纯虚函数\nstruct Animal {\n    virtual ~Animal() { cleanup(); }    // 析构时调用虚函数\n    virtual void cleanup() = 0;\n};\n\nstruct Dog : Animal {\n    void cleanup() override { std::cout << \"Dog cleanup\\n\"; }\n};\n\nvoid crash_case_2() {\n    Dog d;                    // 析构 Animal 时调用纯虚函数 cleanup() → crash\n}\n\n// ❌ 崩溃：通过未完全构造的对象调用虚函数\nstruct Shape {\n    Shape() { draw(); }\n    virtual void draw() = 0;\n};\n\nvoid crash_case_3() {\n    struct Circle : Shape {\n        void draw() override { std::cout << \"circle\\n\"; }\n    };\n    Circle c;                // 崩溃\n}\n\n// ✅ 修复：用两阶段初始化\nclass SafeBase {\npublic:\n    SafeBase() = default;\n    virtual void init() = 0;\n    void initialize() { init(); }   // 构造完成后手动调用\n};\n\nclass SafeDerived : public SafeBase {\npublic:\n    void init() override { std::cout << \"SafeDerived::init\\n\"; }\n};\n\nvoid fix_case() {\n    SafeDerived d;\n    d.initialize();                // 此时对象已完全构造\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n    fix_case();\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGABRT (6) — pure virtual method called\n *\n * 原因:\n *   C++ 对象构造/析构期间，虚函数表指向当前层级\n *   构造 Base 时，vptr 指向 Base 的 vtable，不会调用 Derived 的重写\n *   如果 Base 中是纯虚函数 → 调用 __cxa_pure_virtual → abort\n *\n * GDB 定位:\n *   bt 会看到 __cxa_pure_virtual → abort\n *\n * 修复方案:\n *   1. 构造函数中不调用虚函数\n *   2. 用两阶段初始化 (先构造，再 init())\n *   3. 用非虚函数 + CRTP (静态多态)\n *   4. 构造函数中调用非虚的私有实现\n *   5. 使用模板方法模式时注意调用时机\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/12_pure_virtual_call.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 20.5s)

_Thinking:_

Let me create the last two crash case files.

**Tool: write**

**Input:**
```json
{
  "content": "// 返回局部变量引用/指针 — C++ 面试崩溃用例 #13\n// 编译: g++ -g -std=c++17 -o 13_return_local_ref 13_return_local_ref.cpp\n\n#include <iostream>\n#include <string>\n\n// ❌ 崩溃：返回局部变量的引用\nstd::string& get_name() {\n    std::string name = \"Alice\";\n    return name;                    // name 在函数返回时被销毁\n}\n\nvoid crash_case_1() {\n    std::string& ref = get_name();\n    std::cout << ref << \"\\n\";       // 未定义行为\n}\n\n// ❌ 崩溃：返回局部变量的指针\nint* get_value() {\n    int x = 42;\n    return &x;                      // x 已被销毁\n}\n\nvoid crash_case_2() {\n    int* p = get_value();\n    std::cout << *p << \"\\n\";        // 未定义行为\n}\n\n// ❌ 崩溃：返回临时对象的引用\nconst std::string& make_upper(const std::string& s) {\n    std::string result = s;\n    for (auto& c : result) c = toupper(c);\n    return result;                  // result 是局部变量\n}\n\nvoid crash_case_3() {\n    const std::string& upper = make_upper(\"hello\");\n    std::cout << upper << \"\\n\";     // 未定义行为\n}\n\n// ✅ 修复：返回值 (现代 C++ 有 RVO/NRVO 优化)\nstd::string get_name_fixed() {\n    std::string name = \"Alice\";\n    return name;                    // RVO/NRVO 优化，无额外拷贝\n}\n\nvoid fix_case_1() {\n    std::string name = get_name_fixed();\n    std::cout << name << \"\\n\";\n}\n\n// ✅ 修复：返回 static (适合单例/常量)\nconst std::string& get_constant() {\n    static const std::string msg = \"Hello, World!\";\n    return msg;\n}\n\nvoid fix_case_2() {\n    const std::string& ref = get_constant();\n    std::cout << ref << \"\\n\";\n}\n\n// ✅ 修复：通过参数传出\nvoid get_value_out(int& out) {\n    out = 42;\n}\n\nvoid fix_case_3() {\n    int value;\n    get_value_out(value);\n    std::cout << value << \"\\n\";\n}\n\nint main() {\n    // 取消注释逐一测试\n    // crash_case_1();\n    // crash_case_2();\n    // crash_case_3();\n\n    fix_case_1();\n    fix_case_2();\n    fix_case_3();\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或静默数据损坏\n *\n * 编译器警告:\n *   -Wreturn-local-addr   GCC/Clang 可检测并警告\n *\n * 关键区分:\n *   返回局部变量值   ✅ OK (有 RVO 优化)\n *   返回局部变量引用 ❌ 悬垂引用\n *   返回局部变量指针 ❌ 悬垂指针\n *   返回 static 引用 ✅ OK (生命周期到程序结束)\n *   返回成员引用     ⚠️ 需确保对象存活\n *   返回临时对象     ❌ 但 const T& 绑定可延长生命周期 (仅限直接绑定)\n *\n * 修复方案:\n *   1. 返回值而非引用 (现代 C++ 零成本)\n *   2. 用 static 延长生命周期 (注意线程安全)\n *   3. 通过输出参数传出\n *   4. 返回智能指针管理动态对象\n *   5. 开启 -Wreturn-local-addr -Werror\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/13_return_local_ref.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 21.7s)

_Thinking:_

Let me create the last crash case file.

**Tool: write**

**Input:**
```json
{
  "content": "// 静态对象析构顺序 — C++ 面试崩溃用例 #14\n// 编译: g++ -g -std=c++17 -o 14_static_destruction_order 14_static_destruction_order.cpp\n\n#include <iostream>\n#include <string>\n\n// ❌ 问题：跨 TU (编译单元) 的静态对象析构顺序不确定\n\n// 模拟一个日志系统 (先构造)\nstruct Logger {\n    Logger() { std::cout << \"Logger constructed\\n\"; }\n    void log(const std::string& msg) {\n        std::cout << \"[LOG] \" << msg << \"\\n\";\n    }\n    ~Logger() {\n        std::cout << \"Logger destroyed\\n\";\n    }\n};\n\n// 模拟数据库连接 (后构造)\nstruct Database {\n    Database() { std::cout << \"Database constructed\\n\"; }\n    void query(const std::string& sql) {\n        logger.log(\"Executing: \" + sql);  // 析构时可能 logger 已被销毁\n    }\n    ~Database() {\n        std::cout << \"Database destroyed\\n\";\n        logger.log(\"DB connection closed\"); // 可能 use-after-free\n    }\n\n    // 引用全局 Logger\n    Logger& logger;\n};\n\n// 全局静态对象 — 构造顺序由定义顺序决定，析构顺序相反\nLogger g_logger;\nDatabase g_db{g_logger};\n\n// 程序退出时:\n//   1. g_db 先析构 → 调用 g_logger.log()\n//   2. g_logger 后析构\n// 如果在同一文件，顺序是有保证的。但跨 .cpp 文件就不确定了。\n\nvoid crash_case() {\n    std::cout << \"running...\\n\";\n    g_db.query(\"SELECT * FROM users\");\n    // 退出时 g_db 析构 → 可能访问已销毁的 g_logger\n}\n\n// ✅ 修复：用函数局部静态 (Meyers Singleton)，保证析构逆序\nLogger& get_logger() {\n    static Logger instance;\n    return instance;\n}\n\nDatabase& get_db() {\n    static Database instance{get_logger()};\n    return instance;\n}\n\nvoid fix_case() {\n    std::cout << \"running with fix...\\n\";\n    get_db().query(\"SELECT * FROM users\");\n    // 局部静态对象按构造逆序析构，保证安全\n}\n\nint main() {\n    // crash_case();\n    fix_case();\n    return 0;\n}\n\n/*\n * ====== 面试要点 ======\n *\n * 崩溃信号: SIGSEGV (11) 或静默损坏\n *\n * 问题: 静态对象析构顺序 (Static Deinitialization Order Fiasco)\n *   同一编译单元: 析构顺序 = 构造顺序的逆序 ✅\n *   不同编译单元: 析构顺序不确定 ❌\n *\n * C++ 标准规定:\n *   函数局部静态对象按构造逆序析构\n *   命名空间作用域静态对象: 同 TU 确定，跨 TU 不确定\n *\n * 修复方案:\n *   1. Meyers Singleton (函数局部静态)\n *   2. 避免在析构函数中依赖其他全局对象\n *   3. 用 shutdown() 显式清理顺序\n *   4. 避免全局状态，用依赖注入\n *   5. C++11 后局部静态初始化是线程安全的\n *\n * 相关考点:\n *   - 静态初始化顺序问题 (Static Initialization Order Fiasco)\n *   - N3547 / P0628: default 构造局部静态线程安全\n *   - __attribute__((destructor)) / atexit 的调用顺序\n */\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp"
}
```

**Output:**
```
Wrote file successfully.

LSP errors detected in this file, please fix:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp">
ERROR [22:5] Constructor for 'Database' must explicitly initialize the reference member 'logger'

14_static_destruction_order.cpp:32:13: note: declared here
ERROR [37:10] No matching constructor for initialization of 'Database'

14_static_destruction_order.cpp:21:8: note: candidate constructor (the implicit copy constructor) not viable: no known conversion from 'Logger' to 'const Database' for 1st argument

14_static_destruction_order.cpp:22:5: note: candidate constructor not viable: requires 0 arguments, but 1 was provided
ERROR [57:21] No matching constructor for initialization of 'Database'

14_static_destruction_order.cpp:21:8: note: candidate constructor (the implicit copy constructor) not viable: no known conversion from 'Logger' to 'const Database' for 1st argument

14_static_destruction_order.cpp:22:5: note: candidate constructor not viable: requires 0 arguments, but 1 was provided
</diagnostics>
```

---

## Assistant (Build · Big Pickle · 5.5s)

_Thinking:_

I need to fix the Database constructor to use an initializer list for the reference member.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp",
  "newString": "struct Database {\n    Database(Logger& log) : logger(log) { std::cout << \"Database constructed\\n\"; }",
  "oldString": "struct Database {\n    Database() { std::cout << \"Database constructed\\n\"; }"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.6s)

_Thinking:_

Now I need to fix the other usages of Database constructor.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp",
  "newString": "Database g_db{g_logger};",
  "oldString": "Database g_db{g_logger};"
}
```

**Error:**
```
No changes to apply: oldString and newString are identical.
```

---

## Assistant (Build · Big Pickle · 3.6s)

_Thinking:_

The g_db line should already be fine since it passes g_logger. Let me re-read the file to check the current state.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/14_static_destruction_order.cpp</path>
<type>file</type>
<content>
1: // 静态对象析构顺序 — C++ 面试崩溃用例 #14
2: // 编译: g++ -g -std=c++17 -o 14_static_destruction_order 14_static_destruction_order.cpp
3: 
4: #include <iostream>
5: #include <string>
6: 
7: // ❌ 问题：跨 TU (编译单元) 的静态对象析构顺序不确定
8: 
9: // 模拟一个日志系统 (先构造)
10: struct Logger {
11:     Logger() { std::cout << "Logger constructed\n"; }
12:     void log(const std::string& msg) {
13:         std::cout << "[LOG] " << msg << "\n";
14:     }
15:     ~Logger() {
16:         std::cout << "Logger destroyed\n";
17:     }
18: };
19: 
20: // 模拟数据库连接 (后构造)
21: struct Database {
22:     Database(Logger& log) : logger(log) { std::cout << "Database constructed\n"; }
23:     void query(const std::string& sql) {
24:         logger.log("Executing: " + sql);  // 析构时可能 logger 已被销毁
25:     }
26:     ~Database() {
27:         std::cout << "Database destroyed\n";
28:         logger.log("DB connection closed"); // 可能 use-after-free
29:     }
30: 
31:     // 引用全局 Logger
32:     Logger& logger;
33: };
34: 
35: // 全局静态对象 — 构造顺序由定义顺序决定，析构顺序相反
36: Logger g_logger;
37: Database g_db{g_logger};
38: 
39: // 程序退出时:
40: //   1. g_db 先析构 → 调用 g_logger.log()
41: //   2. g_logger 后析构
42: // 如果在同一文件，顺序是有保证的。但跨 .cpp 文件就不确定了。
43: 
44: void crash_case() {
45:     std::cout << "running...\n";
46:     g_db.query("SELECT * FROM users");
47:     // 退出时 g_db 析构 → 可能访问已销毁的 g_logger
48: }
49: 
50: // ✅ 修复：用函数局部静态 (Meyers Singleton)，保证析构逆序
51: Logger& get_logger() {
52:     static Logger instance;
53:     return instance;
54: }
55: 
56: Database& get_db() {
57:     static Database instance{get_logger()};
58:     return instance;
59: }
60: 
61: void fix_case() {
62:     std::cout << "running with fix...\n";
63:     get_db().query("SELECT * FROM users");
64:     // 局部静态对象按构造逆序析构，保证安全
65: }
66: 
67: int main() {
68:     // crash_case();
69:     fix_case();
70:     return 0;
71: }
72: 
73: /*
74:  * ====== 面试要点 ======
75:  *
76:  * 崩溃信号: SIGSEGV (11) 或静默损坏
77:  *
78:  * 问题: 静态对象析构顺序 (Static Deinitialization Order Fiasco)
79:  *   同一编译单元: 析构顺序 = 构造顺序的逆序 ✅
80:  *   不同编译单元: 析构顺序不确定 ❌
81:  *
82:  * C++ 标准规定:
83:  *   函数局部静态对象按构造逆序析构
84:  *   命名空间作用域静态对象: 同 TU 确定，跨 TU 不确定
85:  *
86:  * 修复方案:
87:  *   1. Meyers Singleton (函数局部静态)
88:  *   2. 避免在析构函数中依赖其他全局对象
89:  *   3. 用 shutdown() 显式清理顺序
90:  *   4. 避免全局状态，用依赖注入
91:  *   5. C++11 后局部静态初始化是线程安全的
92:  *
93:  * 相关考点:
94:  *   - 静态初始化顺序问题 (Static Initialization Order Fiasco)
95:  *   - N3547 / P0628: default 构造局部静态线程安全
96:  *   - __attribute__((destructor)) / atexit 的调用顺序
97:  */

(End of file - total 97 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 13.2s)

_Thinking:_

The code looks correct now with the initializer list for the reference member. Let me compile all the files to verify they work correctly.

Let me compile all the files to verify they work:

**Tool: bash**

**Input:**
```json
{
  "command": "cd /Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases && g++ -g -std=c++17 -pthread -o 01_null_pointer 01_null_pointer.cpp && g++ -g -std=c++17 -pthread -o 02_dangling_pointer 02_dangling_pointer.cpp && g++ -g -std=c++17 -pthread -o 03_buffer_overflow 03_buffer_overflow.cpp && g++ -g -std=c++17 -pthread -o 04_stack_overflow 04_stack_overflow.cpp && g++ -g -std=c++17 -pthread -o 05_double_free 05_double_free.cpp && g++ -g -std=c++17 -pthread -o 06_uninitialized_ptr 06_uninitialized_ptr.cpp && g++ -g -std=c++17 -pthread -o 07_division_by_zero 07_division_by_zero.cpp && g++ -g -std=c++17 -pthread -o 08_out_of_bounds 08_out_of_bounds.cpp && echo \"First 8 compiled OK\"",
  "description": "Compile first 8 crash case files"
}
```

**Output:**
```
02_dangling_pointer.cpp:18:13: warning: address of stack memory associated with local variable 'x' returned [-Wreturn-stack-address]
   18 |     return &x;                  // x 在函数返回后被销毁
      |             ^
1 warning generated.
03_buffer_overflow.cpp:11:5: warning: 'strcpy' will always overflow; destination buffer has size 4, but the source string has length 27 (including NUL byte) [-Wfortify-source]
   11 |     strcpy(buf, "this is a very long string");  // 溢出，覆盖栈上其他数据
      |     ^
03_buffer_overflow.cpp:28:5: warning: array index 5 is past the end of the array (that has type 'char[5]') [-Warray-bounds]
   28 |     buf[5] = '\0';               // off-by-one 写入
      |     ^   ~
03_buffer_overflow.cpp:24:5: note: array 'buf' declared here
   24 |     char buf[5];
      |     ^
2 warnings generated.
08_out_of_bounds.cpp:12:18: warning: array index 10 is past the end of the array (that has type 'int[5]') [-Warray-bounds]
   12 |     std::cout << arr[10] << "\n";     // 越界读取
      |                  ^   ~~
08_out_of_bounds.cpp:11:5: note: array 'arr' declared here
   11 |     int arr[5] = {1, 2, 3, 4, 5};
      |     ^
08_out_of_bounds.cpp:13:5: warning: array index 10 is past the end of the array (that has type 'int[5]') [-Warray-bounds]
   13 |     arr[10] = 99;                     // 越界写入
      |     ^   ~~
08_out_of_bounds.cpp:11:5: note: array 'arr' declared here
   11 |     int arr[5] = {1, 2, 3, 4, 5};
      |     ^
2 warnings generated.
First 8 compiled OK

```

---

## Assistant (Build · Big Pickle · 9.8s)

_Thinking:_

Good, those warnings are expected since these are crash case examples. Let me compile the remaining files.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases && g++ -g -std=c++17 -pthread -o 09_data_race 09_data_race.cpp && g++ -g -std=c++17 -pthread -o 10_deadlock 10_deadlock.cpp && g++ -g -std=c++17 -pthread -o 11_iterator_invalidate 11_iterator_invalidate.cpp && g++ -g -std=c++17 -pthread -o 12_pure_virtual_call 12_pure_virtual_call.cpp && g++ -g -std=c++17 -pthread -o 13_return_local_ref 13_return_local_ref.cpp && g++ -g -std=c++17 -pthread -o 14_static_destruction_order 14_static_destruction_order.cpp && echo \"All 6 remaining compiled OK\"",
  "description": "Compile remaining crash case files"
}
```

**Output:**
```
12_pure_virtual_call.cpp:9:9: warning: call to pure virtual member function 'init' has undefined behavior; overrides of 'init' in subclasses are not available in the constructor of 'Base' [-Wcall-to-pure-virtual-from-ctor-dtor]
    9 |         init();               // 构造期间调用虚函数，调用的是 Base::init 而非 Derived::init
      |         ^
12_pure_virtual_call.cpp:11:5: note: 'init' declared here
   11 |     virtual void init() = 0;  // 纯虚函数 → 崩溃
      |     ^
12_pure_virtual_call.cpp:25:25: warning: call to pure virtual member function 'cleanup' has undefined behavior; overrides of 'cleanup' in subclasses are not available in the destructor of 'Animal' [-Wcall-to-pure-virtual-from-ctor-dtor]
   25 |     virtual ~Animal() { cleanup(); }    // 析构时调用虚函数
      |                         ^
12_pure_virtual_call.cpp:26:5: note: 'cleanup' declared here
   26 |     virtual void cleanup() = 0;
      |     ^
12_pure_virtual_call.cpp:39:15: warning: call to pure virtual member function 'draw' has undefined behavior; overrides of 'draw' in subclasses are not available in the constructor of 'Shape' [-Wcall-to-pure-virtual-from-ctor-dtor]
   39 |     Shape() { draw(); }
      |               ^
12_pure_virtual_call.cpp:40:5: note: 'draw' declared here
   40 |     virtual void draw() = 0;
      |     ^
3 warnings generated.
13_return_local_ref.cpp:10:12: warning: reference to stack memory associated with local variable 'name' returned [-Wreturn-stack-address]
   10 |     return name;                    // name 在函数返回时被销毁
      |            ^~~~
13_return_local_ref.cpp:21:13: warning: address of stack memory associated with local variable 'x' returned [-Wreturn-stack-address]
   21 |     return &x;                      // x 已被销毁
      |             ^
13_return_local_ref.cpp:33:12: warning: reference to stack memory associated with local variable 'result' returned [-Wreturn-stack-address]
   33 |     return result;                  // result 是局部变量
      |            ^~~~~~
3 warnings generated.
All 6 remaining compiled OK

```

---

## Assistant (Build · Big Pickle · 4.3s)

_Thinking:_

All files compiled successfully (warnings are expected since these are crash examples). Let me verify the directory structure.

**Tool: bash**

**Input:**
```json
{
  "command": "ls -la /Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/",
  "description": "List all files in crash_cases directory"
}
```

**Output:**
```
total 1952
drwxr-xr-x@ 45 caviar  staff    1440 May  9 01:13 .
drwxr-xr-x@  3 caviar  staff      96 May  9 01:08 ..
-rwxr-xr-x@  1 caviar  staff   58672 May  9 01:12 01_null_pointer
-rw-r--r--@  1 caviar  staff    1742 May  9 01:08 01_null_pointer.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 01_null_pointer.dSYM
-rwxr-xr-x@  1 caviar  staff   63824 May  9 01:13 02_dangling_pointer
-rw-r--r--@  1 caviar  staff    2014 May  9 01:08 02_dangling_pointer.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 02_dangling_pointer.dSYM
-rwxr-xr-x@  1 caviar  staff   41984 May  9 01:13 03_buffer_overflow
-rw-r--r--@  1 caviar  staff    1869 May  9 01:09 03_buffer_overflow.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 03_buffer_overflow.dSYM
-rwxr-xr-x@  1 caviar  staff   42016 May  9 01:13 04_stack_overflow
-rw-r--r--@  1 caviar  staff    1366 May  9 01:09 04_stack_overflow.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 04_stack_overflow.dSYM
-rwxr-xr-x@  1 caviar  staff   42544 May  9 01:13 05_double_free
-rw-r--r--@  1 caviar  staff    1851 May  9 01:09 05_double_free.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 05_double_free.dSYM
-rwxr-xr-x@  1 caviar  staff   42056 May  9 01:13 06_uninitialized_ptr
-rw-r--r--@  1 caviar  staff    1682 May  9 01:09 06_uninitialized_ptr.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 06_uninitialized_ptr.dSYM
-rwxr-xr-x@  1 caviar  staff   41840 May  9 01:13 07_division_by_zero
-rw-r--r--@  1 caviar  staff    1305 May  9 01:10 07_division_by_zero.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 07_division_by_zero.dSYM
-rwxr-xr-x@  1 caviar  staff   60720 May  9 01:13 08_out_of_bounds
-rw-r--r--@  1 caviar  staff    1936 May  9 01:10 08_out_of_bounds.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 08_out_of_bounds.dSYM
-rwxr-xr-x@  1 caviar  staff  135552 May  9 01:13 09_data_race
-rw-r--r--@  1 caviar  staff    2763 May  9 01:10 09_data_race.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 09_data_race.dSYM
-rwxr-xr-x@  1 caviar  staff   87432 May  9 01:13 10_deadlock
-rw-r--r--@  1 caviar  staff    3401 May  9 01:11 10_deadlock.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 10_deadlock.dSYM
-rwxr-xr-x@  1 caviar  staff  146104 May  9 01:13 11_iterator_invalidate
-rw-r--r--@  1 caviar  staff    3415 May  9 01:11 11_iterator_invalidate.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 11_iterator_invalidate.dSYM
-rwxr-xr-x@  1 caviar  staff   45512 May  9 01:13 12_pure_virtual_call
-rw-r--r--@  1 caviar  staff    2506 May  9 01:11 12_pure_virtual_call.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 12_pure_virtual_call.dSYM
-rwxr-xr-x@  1 caviar  staff   47504 May  9 01:13 13_return_local_ref
-rw-r--r--@  1 caviar  staff    2788 May  9 01:12 13_return_local_ref.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 13_return_local_ref.dSYM
-rwxr-xr-x@  1 caviar  staff   46840 May  9 01:13 14_static_destruction_order
-rw-r--r--@  1 caviar  staff    2850 May  9 01:12 14_static_destruction_order.cpp
drwxr-xr-x@  3 caviar  staff      96 May  9 01:13 14_static_destruction_order.dSYM
-rw-r--r--@  1 caviar  staff    2622 May  9 01:08 README.md

```

---

## Assistant (Build · Big Pickle · 11.1s)

_Thinking:_

All files compiled successfully. Let me verify the directory structure is complete and show the user the final result.

目录已创建，14 个崩溃用例全部编译通过。结构如下：

```
crash_cases/
├── README.md                        # 总览 + 定位手段 + 面试答题模板
├── 01_null_pointer.cpp              # 空指针解引用 (SIGSEGV)
├── 02_dangling_pointer.cpp          # 悬垂指针 / Use After Free
├── 03_buffer_overflow.cpp           # 缓冲区溢出 (栈/堆)
├── 04_stack_overflow.cpp            # 栈溢出 (递归/大数组)
├── 05_double_free.cpp               # 重复释放 (SIGABRT)
├── 06_uninitialized_ptr.cpp         # 未初始化指针
├── 07_division_by_zero.cpp          # 除零错误 (SIGFPE)
├── 08_out_of_bounds.cpp             # 数组/容器越界
├── 09_data_race.cpp                 # 数据竞争 (含 TSan 示例)
├── 10_deadlock.cpp                  # 死锁 (含 scoped_lock 修复)
├── 11_iterator_invalidate.cpp       # 迭代器失效规则
├── 12_pure_virtual_call.cpp         # 纯虚函数调用 (构造/析构期间)
├── 13_return_local_ref.cpp          # 返回局部变量引用/指针
└── 14_static_destruction_order.cpp  # 静态对象析构顺序
```

每个文件包含：
- **崩溃代码** (注释掉，取消注释可复现)
- **修复方案** (可运行的正确代码)
- **面试要点** (崩溃信号、GDB 定位方法、Sanitizer 输出、修复策略)

编译方式：每个 `.cpp` 文件头部有编译命令，直接 `g++ -g -std=c++17 <file>.cpp` 即可。

---

