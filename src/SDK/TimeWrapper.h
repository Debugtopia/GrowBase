#ifndef TIMEWRAPPER_H
#define TIMEWRAPPER_H
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

static std::string GetTimeAsString()
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime = *std::localtime(&time);
    std::stringstream ss;

    ss << std::put_time(&localTime, "%d/%m %H:%M:%S");
    return ss.str();
}

static std::string GetSendDateString(std::chrono::steady_clock::time_point timer)
{
    auto now = std::chrono::steady_clock::now();
    auto system_time = std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(timer - now);
    std::time_t time_t_system_time = std::chrono::system_clock::to_time_t(system_time);
    std::tm local_tm = *std::localtime(&time_t_system_time);
    std::ostringstream oss;

    oss << "(" << std::put_time(&local_tm, "%H:%M on %m/%d") << ")";
    return oss.str();
}

#endif TIMEWRAPPER_H