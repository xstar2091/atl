#include <iostream>

#include "atl/utils/time_string.h"

void test_to_string_default() {
    std::cout<<"-----------------to_string_default-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToString()<<std::endl;
}

void test_to_string_format() {
    std::cout<<"-----------------to_string_format-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToString("20000102030405")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123456")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123456789")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456789")<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456.789")<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123456")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123456789")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456789")<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456~789")<<std::endl;
}

void test_to_string_time_t() {
    std::cout<<"-----------------to_string_time_t-----------------"<<std::endl;
    atl::TimeString ts;
    time_t current_time = time(nullptr);

    std::cout<<ts.ToString("20000102030405", current_time)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05", current_time)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05", current_time)<<std::endl;
}

void test_to_string_time_point() {
    std::cout<<"-----------------to_string_time_point-----------------"<<std::endl;
    atl::TimeString ts;
    auto time_point = std::chrono::system_clock::now();

    std::cout<<ts.ToString("20000102030405", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123456", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123456789", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456789", time_point)<<std::endl;
    std::cout<<ts.ToString("2000-01-02 03:04:05.123.456.789", time_point)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123456", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123456789", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456789", time_point)<<std::endl;
    std::cout<<ts.ToString("2000/01/02 03-04-05~123~456~789", time_point)<<std::endl;
}

void test_to_date_string_default() {
    std::cout<<"-----------------to_date_string_default-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToDateString()<<std::endl;
}

void test_to_date_string_format() {
    std::cout<<"-----------------to_date_string_format-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToDateString("20000102")<<std::endl;
    std::cout<<ts.ToDateString("2000-01-02")<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToDateString("2000/01/02")<<std::endl;
}

void test_to_date_string_time_t() {
    std::cout<<"-----------------to_date_string_time_t-----------------"<<std::endl;
    atl::TimeString ts;
    time_t current_time = time(nullptr);

    std::cout<<ts.ToDateString("20000102", current_time)<<std::endl;
    std::cout<<ts.ToDateString("2000-01-02", current_time)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToDateString("2000/01/02", current_time)<<std::endl;
}

void test_to_date_string_time_point() {
    std::cout<<"-----------------to_date_string_time_point-----------------"<<std::endl;
    atl::TimeString ts;
    auto time_point = std::chrono::system_clock::now();

    std::cout<<ts.ToDateString("20000102", time_point)<<std::endl;
    std::cout<<ts.ToDateString("2000-01-02", time_point)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToDateString("2000/01/02", time_point)<<std::endl;
}

void test_to_time_string_default() {
    std::cout<<"-----------------to_time_string_default-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToTimeString()<<std::endl;
}

void test_to_time_string_format() {
    std::cout<<"-----------------to_time_string_format-----------------"<<std::endl;
    atl::TimeString ts;

    std::cout<<ts.ToTimeString("030405")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123456")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123456789")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456789")<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456.789")<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123456")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123456789")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456789")<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456~789")<<std::endl;
}

void test_to_time_string_time_t() {
    std::cout<<"-----------------to_time_string_time_t-----------------"<<std::endl;
    atl::TimeString ts;
    time_t current_time = time(nullptr);

    std::cout<<ts.ToTimeString("030405", current_time)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05", current_time)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05", current_time)<<std::endl;
}

void test_to_time_string_time_point() {
    std::cout<<"-----------------to_time_string_time_point-----------------"<<std::endl;
    atl::TimeString ts;
    auto time_point = std::chrono::system_clock::now();

    std::cout<<ts.ToTimeString("030405", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123456", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123456789", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456789", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03:04:05.123.456.789", time_point)<<std::endl;

    std::cout<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123456", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123456789", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456789", time_point)<<std::endl;
    std::cout<<ts.ToTimeString("03-04-05~123~456~789", time_point)<<std::endl;
}

int main() {
    test_to_string_default();
    test_to_string_format();
    test_to_string_time_t();
    test_to_string_time_point();
    test_to_date_string_default();
    test_to_date_string_format();
    test_to_date_string_time_t();
    test_to_date_string_time_point();
    test_to_time_string_default();
    test_to_time_string_format();
    test_to_time_string_time_t();
    test_to_time_string_time_point();
    return 0;
}
