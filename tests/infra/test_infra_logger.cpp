#include "infra/log/Logger.hpp"

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

int main()
{
    using namespace Infrastructure::Log;
    namespace fs = std::filesystem;

    const fs::path logs_dir = executable_dir() / "logs";
    const fs::path log_file = logs_dir / "test.log";

    fs::create_directories(logs_dir);

    if (fs::exists(log_file))
        fs::remove(log_file);

    for (auto &p : fs::directory_iterator(logs_dir)) {
        const auto name = p.path().filename().string();
        if (name.starts_with("test ") && p.path().extension() == ".log") {
            fs::remove(p.path());
        }
    }

    Logger::instance().init_file("test.log");

    LOG(INFO) << "hello";
    LOG(ERROR) << "oops";
    LOG(DEBUG) << "dbg";

    assert(fs::exists(log_file));

    std::ifstream in(log_file);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    assert(contents.find("hello") != std::string::npos);
    assert(contents.find("oops") != std::string::npos);
    assert(contents.find("dbg") != std::string::npos);

    std::ostringstream tid;
    tid << std::this_thread::get_id();
    assert(contents.find(tid.str()) != std::string::npos);

    std::regex ts_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    std::smatch match;
    assert(std::regex_search(contents, match, ts_regex));

    Logger::instance().init_file("test.log");
    assert(fs::exists(log_file));

    size_t rotated_count = 0;
    for (auto &p : fs::directory_iterator(logs_dir)) {
        const auto name = p.path().filename().string();
        if (name.starts_with("test ") && p.path().extension() == ".log")
            ++rotated_count;
    }

    assert(rotated_count >= 1);

    return 0;
}
