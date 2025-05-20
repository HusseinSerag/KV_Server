#pragma once
#include "exception.h"
#include "Response.h"
#include <string>
class TypeMismatchException: public BaseException {
    public: 
    TypeMismatchException(const std::string& cmd);
};