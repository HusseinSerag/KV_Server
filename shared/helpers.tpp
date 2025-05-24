
#include <iostream>
 #include <stdio.h>
#include <errno.h>
#include <string.h>
#include "helpers.h"



    template <typename T>
    bool Helper::read_int(const uint8_t* &start, const uint8_t *end, T& out, int stride)
    {
    if(start + stride > end){
        return false;
    }
    memcpy(&out,start, stride);
    start += stride;
    return true;
}



template<>
 inline double Helper::convert(const std::string& str){
    return std::stod(str);
}
template<>
 inline int64_t Helper::convert(const std::string& str){
    return std::stoll(str);
}
template<>
 inline std::string Helper::convert(const std::string& str){
    return str;
}
