#pragma once
#include "exception.h"
#include "Response.h"
class NotFoundException: public BaseException {
    public: 
    NotFoundException();
};