
#include "exception.h"
#include "Response.h"
#include "NotFoundException.h"

     NotFoundException::NotFoundException(): BaseException("key not found!",NOT_FOUND) {}
