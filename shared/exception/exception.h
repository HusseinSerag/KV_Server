#pragma once
#include <exception>
#include <string>
#include "Response.h"

class BaseException: public std::exception {

    std::string message;
    enum ResponseStatus resp;

    public:
        explicit BaseException(const std::string& msg, enum ResponseStatus resp);
        const char* what() const noexcept override;
        enum ResponseStatus getResponse() const;

};
