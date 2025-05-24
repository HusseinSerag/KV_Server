#include "TypeMismatchException.h"
#include "exception.h"
TypeMismatchException::TypeMismatchException(const std::string& cmd, const std::string& type): BaseException("Type mismatch: command '"+cmd+"' is not valid for " +type,ERROR) {}