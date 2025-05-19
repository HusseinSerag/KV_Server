#include "WrongCommandException.h"
#include "exception.h"
WrongCommandException::WrongCommandException(const std::string& msg):BaseException(msg, ERROR) {}