# C++ 进程崩溃用例 & 面试指南

## 目录结构

| 文件 | 崩溃类型 |
|------|----------|
| `01_null_pointer.cpp` | 空指针解引用 |
| `02_dangling_pointer.cpp` | 悬垂指针 / Use After Free |
| `03_buffer_overflow.cpp` | 缓冲区溢出 |
| `04_stack_overflow.cpp` | 栈溢出 |
| `05_double_free.cpp` | 重复释放 |
| `06_uninitialized_ptr.cpp` | 未初始化指针 |
| `07_division_by_zero.cpp` | 除零错误 |
| `08_out_of_bounds.cpp` | 数组越界 |
| `09_data_race.cpp` | 数据竞争 |
| `10_deadlock.cpp` | 死锁 |
| `11_iterator_invalidate.cpp` | 迭代器失效 |
| `12_pure_virtual_call.cpp` | 纯虚函数调用 |
| `13_return_local_ref.cpp` | 返回局部变量引用 |
| `14_static_destruction_order.cpp` | 静态对象析构顺序 |

## 定位手段（面试高频）

### 1. GDB 调试
```bash
gdb -ex run -ex bt ./program          # 运行并打印回溯
gdb -ex "set pagination off" -ex run -ex "info registers" -ex bt ./program
gdb ./program core.xxx                # 分析 core dump
```

### 2. Core Dump 分析
```bash
ulimit -c unlimited                   # 开启 core dump
echo "/tmp/core.%e.%p" > /proc/sys/kernel/core_pattern
gdb ./program /tmp/core.xxx           # 加载分析
```

### 3. AddressSanitizer (ASan)
```bash
g++ -fsanitize=address -g -o prog prog.cpp    # 编译
./prog                                         # 运行即报
```
能检测：内存越界、Use-After-Free、Double-Free、内存泄漏

### 4. UndefinedBehaviorSanitizer (UBSan)
```bash
g++ -fsanitize=undefined -g -o prog prog.cpp
```
能检测：未定义行为（空指针、对齐错误、整型溢出等）

### 5. ThreadSanitizer (TSan)
```bash
g++ -fsanitize=thread -g -o prog prog.cpp
```
能检测：数据竞争

### 6. Valgrind
```bash
valgrind --leak-check=full --track-origins=yes ./program
```

### 7. strace / ltrace
```bash
strace -f ./program       # 跟踪系统调用
ltrace ./program          # 跟踪库函数调用
```

### 8. pstack / gdb attach
```bash
gdb -p <pid> -ex "thread apply all bt" -ex quit    # 看所有线程栈
pstack <pid>
```

## 面试答题模板

当面试官问 **"进程崩溃怎么排查？"** 按以下顺序回答：

1. **看日志** — 应用日志、系统日志（`dmesg` 看 OOM/segfault）
2. **看 Core Dump** — `gdb` 加载 core，`bt` 看调用栈
3. **复现** — 用 ASan/UBSan 编译，快速定位
4. **多线程问题** — 用 TSan、加日志、看线程状态
5. **生产环境** — `gdb attach`、`strace`、检查资源（内存/FD）
6. **预防** — 代码审查、静态分析（clang-tidy）、CI 集成 Sanitizer
