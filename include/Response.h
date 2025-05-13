#pragma once
#include <string>
enum ResponseStatus {

    NOT_FOUND = 404, OK = 200, ERROR = 400

};
class Response {
    public:
    enum ResponseStatus status;
    std::string output;


    Response();
};
