#include "exception.h"
#include <string>
#include "Response.h"

BaseException::BaseException(const std::string& msg, enum ResponseStatus resp) : message(msg), resp(resp) {}

const char* BaseException::what() const noexcept{
    return message.c_str();
}

enum ResponseStatus BaseException::getResponse() const {
    return resp;
}
