
#include <iostream>
 #include <stdio.h>
#include <errno.h>
#include <string.h>



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


