#ifndef TIMEWRAPPER_H
#define TIMEWRAPPER_H
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

static nova_str GetTimeAsString()
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm local_time = *std::localtime(&time);
    nova_sstream ss;

    ss << std::put_time(&local_time, "%d/%m %H:%M:%S");
    return ss.str();
}

static nova_str GetSendDateString(std::chrono::steady_clock::time_point timer)
{
    auto system_time = std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(timer - std::chrono::steady_clock::now());
    std::time_t system_time_t = std::chrono::system_clock::to_time_t(system_time);
    std::tm local_time = *std::localtime(&system_time_t);
    nova_ostream stream;

    stream << "(" << std::put_time(&local_time, "%H:%M on %m/%d") << ")";
    return stream.str();
}

#endif TIMEWRAPPER_H