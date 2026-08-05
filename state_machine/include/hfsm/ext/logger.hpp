#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

namespace hfsm {

    // ============================================================
    // Logger
    // ============================================================

    enum class LogLevel : uint8_t {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warn = 3,
        Error = 4,
        None = 5,
    };

    class Logger
    {
    public:
        using OutputFn = std::function<void(const std::string&)>;

        static Logger& instance()
        {
            static Logger inst;
            return inst;
        }

        void set_level(LogLevel level)
        {
            min_level_ = level;
        }
        LogLevel level() const
        {
            return min_level_;
        }

        void set_output(OutputFn fn)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            output_ = std::move(fn);
        }

        void
        log(LogLevel level, const std::string& module, const std::string& msg)
        {
            if (level < min_level_)
                return;

            auto now = std::chrono::system_clock::now();
            auto tt = std::chrono::system_clock::to_time_t(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) %
                      1000;

            std::ostringstream oss;
            oss << "[" << level_label(level) << "]"
                << "[" << std::put_time(std::gmtime(&tt), "%H:%M:%S") << "."
                << std::setfill('0') << std::setw(3) << ms.count() << "]"
                << "[" << module << "] " << msg;

            std::lock_guard<std::mutex> lock(mutex_);
            if (output_) {
                output_(oss.str());
            }
        }

        void trace(const std::string& module, const std::string& msg)
        {
            log(LogLevel::Trace, module, msg);
        }
        void debug(const std::string& module, const std::string& msg)
        {
            log(LogLevel::Debug, module, msg);
        }
        void info(const std::string& module, const std::string& msg)
        {
            log(LogLevel::Info, module, msg);
        }
        void warn(const std::string& module, const std::string& msg)
        {
            log(LogLevel::Warn, module, msg);
        }
        void error(const std::string& module, const std::string& msg)
        {
            log(LogLevel::Error, module, msg);
        }

        static const char* level_label(LogLevel level)
        {
            switch (level) {
            case LogLevel::Trace:
                return "TRACE";
            case LogLevel::Debug:
                return "DEBUG";
            case LogLevel::Info:
                return "INFO";
            case LogLevel::Warn:
                return "WARN";
            case LogLevel::Error:
                return "ERROR";
            case LogLevel::None:
                return "NONE";
            }
            return "????";
        }

    private:
        Logger() : min_level_(LogLevel::Info)
        {
            output_ = [](const std::string& s) { printf("%s\n", s.c_str()); };
        }

        LogLevel min_level_;
        OutputFn output_;
        std::mutex mutex_;
    };

    /// Scoped module logger
    class ModuleLogger
    {
    public:
        ModuleLogger(const char* module) : module_(module) {}

        template <typename... Args> void trace(const char* fmt, Args&&... args)
        {
            Logger::instance().trace(module_,
                                     format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args> void debug(const char* fmt, Args&&... args)
        {
            Logger::instance().debug(module_,
                                     format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args> void info(const char* fmt, Args&&... args)
        {
            Logger::instance().info(module_,
                                    format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args> void warn(const char* fmt, Args&&... args)
        {
            Logger::instance().warn(module_,
                                    format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args> void error(const char* fmt, Args&&... args)
        {
            Logger::instance().error(module_,
                                     format(fmt, std::forward<Args>(args)...));
        }

    private:
        template <typename... Args>
        static std::string format(const char* fmt, Args&&... args)
        {
            if constexpr (sizeof...(args) == 0) {
                return fmt;
            }
            else {
                int sz = snprintf(nullptr, 0, fmt, std::forward<Args>(args)...);
                if (sz <= 0)
                    return fmt;
                std::string buf(static_cast<std::size_t>(sz), '\0');
                snprintf(buf.data(), static_cast<std::size_t>(sz) + 1, fmt,
                         std::forward<Args>(args)...);
                return buf;
            }
        }

        const char* module_;
    };

// Convenience macros
#define HFSM_LOG(level, ...)                                                   \
    ::hfsm::Logger::instance().log(::hfsm::LogLevel::level, "HFSM", __VA_ARGS__)

} // namespace hfsm
