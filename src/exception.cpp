#include "exception.h"
#include <string>
#include "Response.h"

StorageException::StorageException(const std::string& msg, enum ResponseStatus resp) : message(msg), resp(resp) {}

const char* StorageException::what() const noexcept {
    return message.c_str();
}

enum ResponseStatus StorageException::getResponse() const {
    return resp;
}
