#include "atl/utils/time_string.h"

#include <cstdio>
#include <cstring>

namespace atl {

namespace {

const char* default_datetime_string_format = "20000102030405";
size_t default_datetime_string_format_size = 14;
const char* default_date_string_format = "20000102";
size_t default_date_string_format_size = 8;
const char* default_time_string_format = "030405";
size_t default_time_string_format_size = 6;
const char* error_invalid_format_string = "invliad format string";

long long int GetMilliseconds(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
}

long long int GetMicroseconds(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
}

long long int GetNanoseconds(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
}

const char* ToStringHelper(const char* fmt,
                           size_t fmt_size,
                           time_t current_time,
                           char* buf,
                           size_t buf_size) {
    struct tm t;
    localtime_r(&current_time, &t);
    switch (fmt_size) {
    case 14:
        {
            strftime(buf, buf_size, "%Y%m%d%H%M%S", &t);
        }
        break;
    case 19:
        {
            strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S", &t);
            buf[4] = fmt[4];
            buf[7] = fmt[7];
            buf[10] = fmt[10];
            buf[13] = fmt[13];
            buf[16] = fmt[16];
        }
        break;
    default:
        return error_invalid_format_string;
    }
    return buf;
}

const char* ToStringHelper(const char* fmt,
                           size_t fmt_size,
                           const std::chrono::system_clock::time_point& current_time_point,
                           char* buf,
                           size_t buf_size) {
    time_t current_time = std::chrono::system_clock::to_time_t(current_time_point);
    switch (fmt_size) {
    case 14:
        return ToStringHelper(fmt, fmt_size, current_time, buf, buf_size);
    case 19:
        return ToStringHelper(fmt, fmt_size, current_time, buf, buf_size);
    }

    struct tm t;
    localtime_r(&current_time, &t);
    char* tail = buf;
    tail += strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S", &t);
    buf[4] = fmt[4];
    buf[7] = fmt[7];
    buf[10] = fmt[10];
    buf[13] = fmt[13];
    buf[16] = fmt[16];
    size_t left = buf_size - (tail - buf);
    switch (fmt_size) {
    case 23:
        {
            long long int milli = GetMilliseconds(current_time_point);
            snprintf(tail, left, ".%03lld", milli);
            tail[0] = fmt[19];
        }
        break;
    case 26:
        {
            long long int micro = GetMicroseconds(current_time_point);
            snprintf(tail, left, ".%06lld", micro);
            tail[0] = fmt[19];
        }
        break;
    case 27:
        {
            long long int micro = GetMicroseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%03lld", micro / 1000, micro % 1000);
            tail[0] = fmt[19];
            tail[4] = fmt[23];
        }
        break;
    case 29:
        {
            long long int nano = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%09lld", nano);
            tail[0] = fmt[19];
        }
        break;
    case 30:
        {
            long long int nano = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%06lld", nano / 1000000, nano % 1000000);
            tail[0] = fmt[19];
            tail[4] = fmt[23];
        }
        break;
    case 31:
        {
            long long int nano = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%03lld.%03lld", nano / 1000000, nano / 1000 % 1000, nano % 1000);
            tail[0] = fmt[19];
            tail[4] = fmt[23];
            tail[8] = fmt[27];
        }
        break;
    default:
        return error_invalid_format_string;
    }
    return buf;
}

const char* ToDateStringHelper(const char* fmt,
                               size_t fmt_size,
                               time_t current_time,
                               char* buf,
                               size_t buf_size) {
    struct tm t;
    localtime_r(&current_time, &t);
    switch (fmt_size) {
    case 8:
        strftime(buf, buf_size, "%Y%m%d", &t);
        break;
    case 10:
        strftime(buf, buf_size, "%Y-%m-%d", &t);
        buf[4] = fmt[4];
        buf[7] = fmt[7];
        break;
    default:
        return error_invalid_format_string;
    }
    return buf;
}

const char* ToDateStringHelper(const char* fmt,
                               size_t fmt_size,
                               const std::chrono::system_clock::time_point& current_time_point,
                               char* buf,
                               size_t buf_size) {
    return ToDateStringHelper(fmt,
                              fmt_size,
                              std::chrono::system_clock::to_time_t(current_time_point),
                              buf,
                              buf_size);
}

const char* ToTimeStringHelper(const char* fmt,
                               size_t fmt_size,
                               time_t current_time,
                               char* buf,
                               size_t buf_size) {
    tm t;
    localtime_r(&current_time, &t);
    switch (fmt_size) {
    case 6:
        strftime(buf, buf_size, "%H%M%S", &t);
        break;
    case 8:
        strftime(buf, buf_size, "%H:%M:%S", &t);
        buf[2] = fmt[2];
        buf[5] = fmt[5];
        break;
    defualt:
        return error_invalid_format_string;
    }
    return buf;
}

const char* ToTimeStringHelper(const char* fmt,
                               size_t fmt_size,
                               const std::chrono::system_clock::time_point& current_time_point,
                               char* buf,
                               size_t buf_size) {
    time_t current_time = std::chrono::system_clock::to_time_t(current_time_point);
    tm t;
    localtime_r(&current_time, &t);
    switch (fmt_size) {
    case 6:
        return ToTimeStringHelper(fmt, fmt_size, current_time, buf, buf_size);
    case 8:
        return ToTimeStringHelper(fmt, fmt_size, current_time, buf, buf_size);
    }
    char* tail = buf;
    tail += strftime(buf, buf_size, "%H:%M:%S", &t);
    buf[2] = fmt[2];
    buf[5] = fmt[5];
    size_t left = buf_size - (tail - buf);
    switch (fmt_size) {
    case 12:
        {
            long long int millisecond = GetMilliseconds(current_time_point);
            snprintf(tail, left, ".%03lld", millisecond);
            tail[0] = fmt[8];
        }
        break;
    case 15:
        {
            long long int microsecond = GetMicroseconds(current_time_point);
            snprintf(tail, left, ".%06lld", microsecond);
            tail[0] = fmt[8];
        }
        break;
    case 16:
        {
            long long int microsecond = GetMicroseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%03lld", microsecond / 1000, microsecond % 1000);
            tail[0] = fmt[8];
            tail[4] = fmt[12];
        }
        break;
    case 18:
        {
            long long int nanosecond = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%09lld", nanosecond);
            tail[0] = fmt[8];
        }
        break;
    case 19:
        {
            long long int nanosecond = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%06lld", nanosecond / 1000000, nanosecond % 1000000);
            tail[0] = fmt[8];
            tail[4] = fmt[12];
        }
        break;
    case 20:
        {
            long long int nanosecond = GetNanoseconds(current_time_point);
            snprintf(tail, left, ".%03lld.%03lld.%03lld",
                     nanosecond / 1000000,
                     nanosecond / 1000 % 1000,
                     nanosecond % 1000);
            tail[0] = fmt[8];
            tail[4] = fmt[12];
            tail[8] = fmt[16];
        }
        break;
    default:
        return error_invalid_format_string;
    }
    return buf;
}

}

const char* TimeString::ToString() {
    return ToStringHelper(default_datetime_string_format,
                          default_datetime_string_format_size,
                          time(nullptr),
                          buf_,
                          sizeof(buf_));
}

const char* TimeString::ToString(const char* fmt) {
    return ToStringHelper(fmt,
                          strlen(fmt),
                          time(nullptr),
                          buf_,
                          sizeof(buf_));
}

const char* TimeString::ToString(const char* fmt, time_t current_time) {
    return ToStringHelper(fmt,
                          strlen(fmt),
                          current_time,
                          buf_,
                          sizeof(buf_));
}

const char* TimeString::ToString(const char* fmt,
                                 const std::chrono::system_clock::time_point& current_time_point) {
    return ToStringHelper(fmt,
                          strlen(fmt),
                          current_time_point,
                          buf_,
                          sizeof(buf_));
}

const char* TimeString::ToDateString() {
    return ToDateStringHelper(default_date_string_format,
                              default_date_string_format_size,
                              time(nullptr),
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToDateString(const char *fmt) {
    return ToDateStringHelper(fmt,
                              strlen(fmt),
                              time(nullptr),
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToDateString(const char *fmt, time_t current_time) {
    return ToDateStringHelper(fmt,
                              strlen(fmt),
                              current_time,
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToDateString(const char *fmt,
                                     const std::chrono::system_clock::time_point& current_time_point) {
    return ToDateStringHelper(fmt,
                              strlen(fmt),
                              current_time_point,
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToTimeString() {
    return ToTimeStringHelper(default_time_string_format,
                              default_time_string_format_size,
                              time(nullptr),
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToTimeString(const char *fmt) {
    return ToTimeStringHelper(fmt,
                              strlen(fmt),
                              time(nullptr),
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToTimeString(const char *fmt, time_t current_time) {
    return ToTimeStringHelper(fmt,
                              strlen(fmt),
                              current_time,
                              buf_,
                              sizeof(buf_));
}

const char* TimeString::ToTimeString(const char *fmt,
                                     const std::chrono::system_clock::time_point& current_time_point) {
    return ToTimeStringHelper(fmt,
                              strlen(fmt),
                              current_time_point,
                              buf_,
                              sizeof(buf_));
}

}
