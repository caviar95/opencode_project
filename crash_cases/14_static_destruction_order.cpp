// 静态对象析构顺序 — C++ 面试崩溃用例 #14
// 编译: g++ -g -std=c++17 -o 14_static_destruction_order 14_static_destruction_order.cpp

#include <iostream>
#include <string>

// ❌ 问题：跨 TU (编译单元) 的静态对象析构顺序不确定

// 模拟一个日志系统 (先构造)
struct Logger {
    Logger() { std::cout << "Logger constructed\n"; }
    void log(const std::string& msg) {
        std::cout << "[LOG] " << msg << "\n";
    }
    ~Logger() {
        std::cout << "Logger destroyed\n";
    }
};

// 模拟数据库连接 (后构造)
struct Database {
    Database(Logger& log) : logger(log) { std::cout << "Database constructed\n"; }
    void query(const std::string& sql) {
        logger.log("Executing: " + sql);  // 析构时可能 logger 已被销毁
    }
    ~Database() {
        std::cout << "Database destroyed\n";
        logger.log("DB connection closed"); // 可能 use-after-free
    }

    // 引用全局 Logger
    Logger& logger;
};

// 全局静态对象 — 构造顺序由定义顺序决定，析构顺序相反
Logger g_logger;
Database g_db{g_logger};

// 程序退出时:
//   1. g_db 先析构 → 调用 g_logger.log()
//   2. g_logger 后析构
// 如果在同一文件，顺序是有保证的。但跨 .cpp 文件就不确定了。

void crash_case() {
    std::cout << "running...\n";
    g_db.query("SELECT * FROM users");
    // 退出时 g_db 析构 → 可能访问已销毁的 g_logger
}

// ✅ 修复：用函数局部静态 (Meyers Singleton)，保证析构逆序
Logger& get_logger() {
    static Logger instance;
    return instance;
}

Database& get_db() {
    static Database instance{get_logger()};
    return instance;
}

void fix_case() {
    std::cout << "running with fix...\n";
    get_db().query("SELECT * FROM users");
    // 局部静态对象按构造逆序析构，保证安全
}

int main() {
    // crash_case();
    fix_case();
    return 0;
}

/*
 * ====== 面试要点 ======
 *
 * 崩溃信号: SIGSEGV (11) 或静默损坏
 *
 * 问题: 静态对象析构顺序 (Static Deinitialization Order Fiasco)
 *   同一编译单元: 析构顺序 = 构造顺序的逆序 ✅
 *   不同编译单元: 析构顺序不确定 ❌
 *
 * C++ 标准规定:
 *   函数局部静态对象按构造逆序析构
 *   命名空间作用域静态对象: 同 TU 确定，跨 TU 不确定
 *
 * 修复方案:
 *   1. Meyers Singleton (函数局部静态)
 *   2. 避免在析构函数中依赖其他全局对象
 *   3. 用 shutdown() 显式清理顺序
 *   4. 避免全局状态，用依赖注入
 *   5. C++11 后局部静态初始化是线程安全的
 *
 * 相关考点:
 *   - 静态初始化顺序问题 (Static Initialization Order Fiasco)
 *   - N3547 / P0628: default 构造局部静态线程安全
 *   - __attribute__((destructor)) / atexit 的调用顺序
 */
