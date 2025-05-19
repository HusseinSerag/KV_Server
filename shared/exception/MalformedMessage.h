#pragma once
#include "exception.h"
#include "Response.h"
class MalformedMessageException: public BaseException {
    public: 
    MalformedMessageException();
};