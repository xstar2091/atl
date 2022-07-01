#include <gtest/gtest.h>

#include "atl/utils/time_string.h"

namespace {

struct tm t;
char expected[32];
const char* actual = nullptr;
atl::TimeString ts;
const char* error_string = "invliad format string";

bool CheckFormat(const char* buf, const std::vector<std::pair<int, char>>& separators) {
    if (!buf) return false;
    size_t buf_len = strlen(buf);
    for (const auto& pair : separators) {
        if (pair.first >= buf_len) return false;
        if (buf[pair.first] != pair.second) return false;
    }
    return true;
}

}

TEST(ToString, Default) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d%H%M%S", &t);
    actual = ts.ToString();
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, Format) {
    actual = ts.ToString("2000-01-02 03:04:05");
    printf("%s\n", actual);
    EXPECT_TRUE(CheckFormat(actual,
                            std::vector<std::pair<int, char>>{ {4, '-'}, {7, '-'}, {10, ' '}, {13, ':'}, {16, ':'} }));
    actual = ts.ToString("2000/01/02T03-04-05");
    printf("%s\n", actual);
    EXPECT_TRUE(CheckFormat(actual,
                            std::vector<std::pair<int, char>>{ {4, '/'}, {7, '/'}, {10, 'T'}, {13, '-'}, {16, '-'} }));
}

TEST(ToString, TimeT14) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d%H%M%S", &t);
    actual = ts.ToString("20000102030405", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimeT19) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    actual = ts.ToString("2000-01-02 03:04:05", current_time);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    actual = ts.ToString("2000/01/02T03-04-05", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimeTError) {
    time_t current_time = time(nullptr);
    actual = ts.ToDateString("2000-01-02 03:04:05.123", current_time);
    EXPECT_STREQ(error_string, actual);
    actual = ts.ToDateString("2000-01-02 03:04:05.123456", current_time);
    EXPECT_STREQ(error_string, actual);
    actual = ts.ToDateString("2000-01-02 03:04:05.123.456", current_time);
    EXPECT_STREQ(error_string, actual);
    actual = ts.ToDateString("2000-01-02 03:04:05.123456789", current_time);
    EXPECT_STREQ(error_string, actual);
    actual = ts.ToDateString("2000-01-02 03:04:05.123.456789", current_time);
    EXPECT_STREQ(error_string, actual);
    actual = ts.ToDateString("2000-01-02 03:04:05.123.456.789", current_time);
    EXPECT_STREQ(error_string, actual);
}

TEST(ToString, TimePoint14) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d%H%M%S", &t);
    actual = ts.ToString("20000102030405", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint19) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    actual = ts.ToString("2000-01-02 03:04:05", time_point);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    actual = ts.ToString("2000/01/02T03-04-05", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint23) {
    auto time_point = std::chrono::system_clock::now();
    long long int millsecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%03lld", millsecond);
    actual = ts.ToString("2000-01-02 03:04:05.123", time_point);
    EXPECT_STRCASEEQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%03lld", millsecond);
    actual = ts.ToString("2000/01/02T03-04-05~123", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint26) {
    auto time_point = std::chrono::system_clock::now();
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%06lld", microsecond);
    actual = ts.ToString("2000-01-02 03:04:05.123456", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%06lld", microsecond);
    actual = ts.ToString("2000/01/02T03-04-05~123456", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint27) {
    auto time_point = std::chrono::system_clock::now();
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%03lld", microsecond / 1000, microsecond % 1000);
    actual = ts.ToString("2000-01-02 03:04:05.123.456", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%03lld", microsecond / 1000, microsecond % 1000);
    actual = ts.ToString("2000/01/02T03-04-05~123~456", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint29) {
    auto time_point = std::chrono::system_clock::now();
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%09lld", nanosecond);
    actual = ts.ToString("2000-01-02 03:04:05.123456789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%09lld", nanosecond);
    actual = ts.ToString("2000/01/02T03-04-05~123456789", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint30) {
    auto time_point = std::chrono::system_clock::now();
    long long int millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%06lld", millisecond, nanosecond % 1000000);
    actual = ts.ToString("2000-01-02 03:04:05.123.456789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%06lld", millisecond, nanosecond % 1000000);
    actual = ts.ToString("2000/01/02T03-04-05~123~456789", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToString, TimePoint31) {
    auto time_point = std::chrono::system_clock::now();
    long long int millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y-%m-%d %H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%03lld.%03lld", millisecond, microsecond % 1000, nanosecond % 1000);
    actual = ts.ToString("2000-01-02 03:04:05.123.456.789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%03lld~%03lld", millisecond, microsecond % 1000, nanosecond % 1000);
    actual = ts.ToString("2000/01/02T03-04-05~123~456~789", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, Default) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d", &t);
    actual = ts.ToDateString();
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, Format08) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d", &t);
    actual = ts.ToDateString("20000102");
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, Format10) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y-%m-%d", &t);
    actual = ts.ToDateString("2000-01-02");
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%Y/%m/%d", &t);
    actual = ts.ToDateString("2000/01/02");
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, TimeT08) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d", &t);
    actual = ts.ToDateString("20000102", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, TimeT10) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y-%m-%d", &t);
    actual = ts.ToDateString("2000-01-02", current_time);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%Y/%m/%d", &t);
    actual = ts.ToDateString("2000/01/02", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, TimePoint08) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y%m%d", &t);
    actual = ts.ToDateString("20000102", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToDateString, TimePoint10) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%Y-%m-%d", &t);
    actual = ts.ToDateString("2000-01-02", time_point);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%Y/%m/%d", &t);
    actual = ts.ToDateString("2000/01/02", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, Default) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H%M%S", &t);
    actual = ts.ToTimeString();
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, Format06) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H%M%S", &t);
    actual = ts.ToTimeString("030405");
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, Format08) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    actual = ts.ToTimeString("03:04:05");
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    actual = ts.ToTimeString("03-04-05");
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimeT06) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H%M%S", &t);
    actual = ts.ToTimeString("030405", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimeT08) {
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    actual = ts.ToTimeString("03:04:05", current_time);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    actual = ts.ToTimeString("03-04-05", current_time);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint06) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H%M%S", &t);
    actual = ts.ToTimeString("030405", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint08) {
    auto time_point = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);

    strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    actual = ts.ToTimeString("03:04:05", time_point);
    EXPECT_STREQ(expected, actual);

    strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    actual = ts.ToTimeString("03-04-05", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint12) {
    auto time_point = std::chrono::system_clock::now();
    long long int millsecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%03lld", millsecond);
    actual = ts.ToTimeString("03:04:05.123", time_point);
    EXPECT_STRCASEEQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%03lld", millsecond);
    actual = ts.ToTimeString("03-04-05~123", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint15) {
    auto time_point = std::chrono::system_clock::now();
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%06lld", microsecond);
    actual = ts.ToTimeString("03:04:05.123456", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%06lld", microsecond);
    actual = ts.ToTimeString("03-04-05~123456", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint16) {
    auto time_point = std::chrono::system_clock::now();
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%03lld", microsecond / 1000, microsecond % 1000);
    actual = ts.ToTimeString("03:04:05.123.456", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%Y/%m/%dT%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%03lld", microsecond / 1000, microsecond % 1000);
    actual = ts.ToString("2000/01/02T03-04-05~123~456", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint18) {
    auto time_point = std::chrono::system_clock::now();
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), ".%09lld", nanosecond);
    actual = ts.ToTimeString("03:04:05.123456789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected), "~%09lld", nanosecond);
    actual = ts.ToTimeString("03-04-05~123456789", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint19) {
    auto time_point = std::chrono::system_clock::now();
    long long int millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%06lld", millisecond, nanosecond % 1000000);
    actual = ts.ToTimeString("03:04:05.123.456789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%06lld", millisecond, nanosecond % 1000000);
    actual = ts.ToTimeString("03-04-05~123~456789", time_point);
    EXPECT_STREQ(expected, actual);
}

TEST(ToTimeString, TimePoint20) {
    auto time_point = std::chrono::system_clock::now();
    long long int millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_point.time_since_epoch()
    ).count() % 1000;
    long long int microsecond = std::chrono::duration_cast<std::chrono::microseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000;
    long long int nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_point.time_since_epoch()
    ).count() % 1000000000;
    time_t current_time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&current_time, &t);
    char* buf = nullptr;

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H:%M:%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        ".%03lld.%03lld.%03lld", millisecond, microsecond % 1000, nanosecond % 1000);
    actual = ts.ToTimeString("03:04:05.123.456.789", time_point);
    EXPECT_STREQ(expected, actual);

    buf = expected;
    buf += strftime(expected, sizeof(expected), "%H-%M-%S", &t);
    snprintf(buf, sizeof(expected) - (buf - expected),
        "~%03lld~%03lld~%03lld", millisecond, microsecond % 1000, nanosecond % 1000);
    actual = ts.ToTimeString("03-04-05~123~456~789", time_point);
    EXPECT_STREQ(expected, actual);
}
