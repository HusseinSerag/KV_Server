#pragma once

#include <string>
#include <deque>

enum class NumberKind {
    NOT_NUMBER, INTEGER, DOUBLE
};
class Helper {
    public:
    static void msg(const char *msg);
    static void msg_errno(const char *msg);
    static void die(const char *msg);
    template <typename T>
    static bool read_int(const uint8_t* &start, const uint8_t *end, T& out, int stride);

    static NumberKind isNumber(const std::string & s);
    static std::string deque_to_string(std::deque<std::string>& dq);
     static bool read_str(const uint8_t* &cur, const uint8_t *end, size_t n, std::string &out);
     // Algorithm produced by Professor Daniel J. Bernstein
     static unsigned int DJBHash(const char* str, unsigned int length);
     static std::string getCurrentTimestamp();
     static int32_t read_full(int fd, char *buf, size_t n);
     static int32_t write_all(int fd, const char *buf, size_t n);
     static bool is_number(const std::string& s);
     template <class T>
     static T convert(const std::string& str);

};


#include "helpers.tpp"

