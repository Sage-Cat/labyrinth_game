#pragma once
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace Infrastructure::Log {

enum class Level : unsigned char { TRACE, DEBUG, INFO, ERROR, FATAL };

#ifndef LABYRINTH_LOG_MIN_LEVEL
inline constexpr Level kMinLevel = Level::DEBUG;
#else
inline constexpr Level kMinLevel = static_cast<Level>(LABYRINTH_LOG_MIN_LEVEL);
#endif

constexpr std::string_view to_string(Level lvl) noexcept
{
    switch (lvl) {
    case Level::TRACE:
        return "TRACE";
    case Level::DEBUG:
        return "DEBUG";
    case Level::INFO:
        return "INFO";
    case Level::ERROR:
        return "ERROR";
    case Level::FATAL:
        return "FATAL";
    }
    return "UNKNOWN";
}

inline std::filesystem::path executable_dir()
{
#if defined(__linux__)
    std::array<char, 4096> buf{};
    const ssize_t len = ::readlink("/proc/self/exe", buf.data(), buf.size() - 1);
    if (len > 0) {
        return std::filesystem::path(std::string(buf.data(), len)).parent_path();
    }
#elif defined(_WIN32)
    char buf[MAX_PATH]{};
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path();
#endif
    return std::filesystem::current_path();
}

class Logger final {
public:
    static Logger &instance()
    {
        static Logger inst;
        return inst;
    }

    Logger(const Logger &)            = delete;
    Logger &operator=(const Logger &) = delete;

    void init_file(const std::string &filename)
    {
        std::scoped_lock lock(m_mtx);

        std::filesystem::path logs_dir = executable_dir() / "logs";
        std::filesystem::create_directories(logs_dir);
        std::filesystem::path log_path = logs_dir / filename;

        if (std::filesystem::exists(log_path)) {
            std::string rotated_name =
                log_path.stem().string() + " " + timestamp(true) + log_path.extension().string();
            std::filesystem::rename(log_path, logs_dir / rotated_name);
        }

        m_out.emplace(log_path, std::ios::out | std::ios::app);

        if (!m_out->is_open()) {
            m_out.reset();
        }
    }

    void set_min_level(Level lvl)
    {
        std::scoped_lock lock(m_mtx);
        m_runtime_min = lvl;
    }

    void write_line(Level lvl, std::string_view file, int line, std::string_view func,
                    std::string_view message)
    {
        if (!should_log(lvl))
            return;

        const auto ts = timestamp(false);

        std::filesystem::path p{file};
        std::ostringstream header;
        header << ts << " [" << to_string(lvl) << "] "
               << "(" << std::this_thread::get_id() << ") " << p.filename().string() << ":" << line
               << " " << func << " - ";

        std::scoped_lock lock(m_mtx);
        if (m_out && m_out->is_open()) {
            (*m_out) << header.str() << message << '\n';
            m_out->flush();
        } else {
            std::cerr << header.str() << message << '\n';
        }

        if (lvl == Level::FATAL) {
            std::abort();
        }
    }

private:
    Logger() = default;

    bool should_log(Level lvl) const noexcept
    {
        const auto min_lvl = m_runtime_min.value_or(kMinLevel);
        return static_cast<unsigned>(lvl) >= static_cast<unsigned>(min_lvl);
    }

    static std::string timestamp(bool for_filename)
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto t   = system_clock::to_time_t(now);

        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::ostringstream oss;
        if (for_filename) {
            oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        } else {
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        }
        return oss.str();
    }

    mutable std::mutex m_mtx;
    std::optional<std::ofstream> m_out{};
    std::optional<Level> m_runtime_min;
};

class Line final {
public:
    Line(Level lvl, std::string_view file, int line, std::string_view func)
        : m_lvl(lvl), m_file(file), m_line(line), m_func(func)
    {
    }

    ~Line()
    {
        if (m_active) {
            Logger::instance().write_line(m_lvl, m_file, m_line, m_func, m_ss.str());
        }
    }

    Line(const Line &)            = delete;
    Line &operator=(const Line &) = delete;

    Line(Line &&other) noexcept
        : m_lvl(other.m_lvl), m_file(other.m_file), m_line(other.m_line), m_func(other.m_func),
          m_ss(std::move(other.m_ss)), m_active(other.m_active)
    {
        other.m_active = false;
    }

    template <class T> Line &operator<<(T &&v)
    {
        m_ss << std::forward<T>(v);
        return *this;
    }

private:
    Level m_lvl;
    std::string_view m_file;
    int m_line;
    std::string_view m_func;
    std::ostringstream m_ss;
    bool m_active{true};
};

#define LOG(level)                                                                                 \
    ::Infrastructure::Log::Line(::Infrastructure::Log::Level::level, __FILE__, __LINE__, __func__)

} // namespace Infrastructure::Log
