#include "TypeMismatchException.h"
#include "exception.h"
TypeMismatchException::TypeMismatchException(const std::string& cmd): BaseException("Type mismatch: command '"+cmd+"' is not valid for string",ERROR) {}