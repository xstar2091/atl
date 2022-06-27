#pragma once

#include <ctime>
#include <chrono>

namespace atl {

class TimeString {
public:
    /**
     * @brief 获取当前时间点的字符串表示
     *
     * 支持的格式
     * 1. 长度: 14 20000102030405
     *
     * @return const char* 日期时间的字符串表示
     */
    const char* ToString();
    /**
     * @brief 获取当前时间点的字符串表示
     *
     * 支持的格式
     * 1. 长度: 14 20000102030405
     * 2. 长度: 19 2000-01-02 03:04:05
     * 3. 长度: 23 2000-01-02 03:04:05.123
     * 4. 长度: 26 2000-01-02 03:04:05.123456
     * 5. 长度: 27 2000-01-02 03:04:05.123.456
     * 6. 长度: 29 2000-01-02 03:04:05.123456789
     * 7. 长度: 30 2000-01-02 03:04:05.123.456789
     * 8. 长度: 31 2000-01-02 03:04:05.123.456.789
     *
     * @param fmt 日期时间格式字符串
     * @return const char* 日期时间的字符串表示
     */
    const char* ToString(const char* fmt);
    /**
     * @brief 获取指定时间点的字符串表示
     * 
     * 支持的格式
     * 1. 长度: 14 20000102030405
     * 2. 长度: 19 2000-01-02 03:04:05
     *
     * @param fmt 日期时间格式字符串
     * @param current_time 指定时间点
     * @return const char* 日期时间的字符串表示
     */
    const char* ToString(const char* fmt, time_t current_time);
    /**
     * @brief 获取指定时间点的字符串表示
     *
     * 支持的格式
     * 1. 长度: 14 20000102030405
     * 2. 长度: 19 2000-01-02 03:04:05
     * 3. 长度: 23 2000-01-02 03:04:05.123
     * 4. 长度: 26 2000-01-02 03:04:05.123456
     * 5. 长度: 27 2000-01-02 03:04:05.123.456
     * 6. 长度: 29 2000-01-02 03:04:05.123456789
     * 7. 长度: 30 2000-01-02 03:04:05.123.456789
     * 8. 长度: 31 2000-01-02 03:04:05.123.456.789
     *
     * @param fmt 日期时间格式字符串
     * @param current_time_point 指定时间点
     * @return const char* 日期时间的字符串表示
     */
    const char* ToString(const char* fmt,
                         const std::chrono::system_clock::time_point& current_time_point);
    /**
     * @brief 获取当前日期的字符串表示
     *
     * 支持的格式
     * 1. 长度: 08 20000102
     *
     * @param fmt 日期格式字符串
     * @return const char* 日期的字符串表示
     */
    const char* ToDateString();
    /**
     * @brief 获取当前日期的字符串表示
     *
     * 支持的格式
     * 1. 长度: 08 20000102
     * 2. 长度: 10 2000-01-02
     *
     * @param fmt 日期格式字符串
     * @return const char* 日期的字符串表示
     */
    const char* ToDateString(const char* fmt);
    /**
     * @brief 获取当前日期的字符串表示
     *
     * 支持的格式
     * 1. 长度: 08 20000102
     * 2. 长度: 10 2000-01-02
     *
     * @param fmt 日期格式字符串
     * @param current_time 指定时间点
     * @return const char* 日期的字符串表示
     */
    const char* ToDateString(const char* fmt, time_t current_time);
    /**
     * @brief 获取当前日期的字符串表示
     *
     * 支持的格式
     * 1. 长度: 08 20000102
     * 2. 长度: 10 2000-01-02
     *
     * @param fmt 日期格式字符串
     * @param current_time_point 指定时间点
     * @return const char* 日期的字符串表示
     */
    const char* ToDateString(const char* fmt,
                             const std::chrono::system_clock::time_point& current_time_point);
    /**
     * @brief 获取当前时间的字符串表示
     *
     * 支持的格式
     * 1. 长度: 06 030405
     *
     * @param fmt 时间格式字符串
     * @param current_time_point 指定时间点
     * @return const char* 时间的字符串表示
     */
    const char* ToTimeString();
    /**
     * @brief 获取当前时间的字符串表示
     *
     * 支持的格式
     * 1. 长度: 06 030405
     * 2. 长度: 08 03:04:05
     * 3. 长度: 12 03:04:05.123
     * 4. 长度: 15 03:04:05.123456
     * 5. 长度: 16 03:04:05.123.456
     * 6. 长度: 18 03:04:05.123456789
     * 7. 长度: 19 03:04:05.123.456789
     * 8. 长度: 20 03:04:05.123.456.789
     *
     * @param fmt 时间格式字符串
     * @return const char* 时间的字符串表示
     */
    const char* ToTimeString(const char* fmt);
    /**
     * @brief 获取当前时间的字符串表示
     *
     * 支持的格式
     * 1. 长度: 06 030405
     * 2. 长度: 08 03-04-05
     *
     * @param fmt 时间格式字符串
     * @return const char* 时间的字符串表示
     */
    const char* ToTimeString(const char* fmt, time_t current_time);
    /**
     * @brief 获取当前时间的字符串表示
     *
     * 支持的格式
     * 1. 长度: 06 030405
     * 2. 长度: 08 03-04-05
     * 3. 长度: 12 03:04:05.123
     * 4. 长度: 15 03:04:05.123456
     * 5. 长度: 16 03:04:05.123.456
     * 6. 长度: 18 03:04:05.123456789
     * 7. 长度: 19 03:04:05.123.456789
     * 8. 长度: 20 03:04:05.123.456.789
     *
     * @param fmt 时间格式字符串
     * @return const char* 时间的字符串表示
     */
    const char* ToTimeString(const char* fmt,
                             const std::chrono::system_clock::time_point& current_time_point);

private:
    char buf_[32];
};

}
