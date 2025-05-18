#include <iostream>
 #include <stdio.h>
#include <errno.h>
#include "helpers.h"
#include <string>
#include <deque>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>

std::string Helper::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "]";
    return ss.str();
}

 void Helper::msg(const char *msg) {
        fprintf(stderr, "%s\n", msg);
    }

    void Helper::msg_errno(const char *msg) {
        fprintf(stderr, "[errno:%d] %s\n", errno, msg);
    }

    void Helper::die(const char *msg) {
        fprintf(stderr, "[%d] %s\n", errno, msg);
        abort();
    }

     bool Helper::read_str(const uint8_t* &cur, const uint8_t *end, size_t n, std::string &out) {
        if (cur + n > end) {
            return false;
        }
        out.assign(cur, cur + n);
        cur += n;
        return true;
    }

    unsigned int Helper::DJBHash(const char* str, unsigned int length){
        unsigned int hash = 5381;
        unsigned int i = 0;
        for(i = 0; i < length; ++str, ++i)
        {
            hash = ((hash << 5) + hash) + (*str);
        }
        return hash;
    }


     std::string Helper::deque_to_string(std::deque<std::string>& dq) {

        std::string out = "";
        bool first = true;

        while (dq.size() > 0) {
            std::string& s = dq.front();
            if (!first) {
                out += ", ";
            }
            out += s;
            first = false;
            dq.pop_front();
        }
        return out;
     }


   int32_t Helper::read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error, or unexpected EOF
        }
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}
 int32_t Helper::write_all(int fd, const char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error
        }
        n -= (size_t)rv;
        buf += rv;

    }
    return 0;
}


 bool Helper::is_number(const std::string& s) {
    if (s.empty()) return false;
    char* end = nullptr;
    std::strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}

NumberKind Helper::isNumber(const std::string &s){
    if(s.empty()) return NumberKind::NOT_NUMBER;
    bool has_dot = false;
    size_t i = 0;
    if(s[i] == '-' || s[i] == '+') i++; // allow sign;
    for(; i < s.size() ; i++){
        if(s[i] == '.'){
            if(has_dot) return NumberKind::NOT_NUMBER;
            has_dot = true;
        } else if(!isdigit(s[i])){
            return NumberKind::NOT_NUMBER;
        }
    }
    if(has_dot) return NumberKind::DOUBLE;
    return NumberKind::INTEGER;
}