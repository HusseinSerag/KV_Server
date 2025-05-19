#pragma once
#include "exception.h"
#include "Response.h"
class WrongCommandException: public BaseException {
    public: 
    WrongCommandException(const std::string& msg);
};