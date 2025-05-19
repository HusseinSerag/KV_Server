
#include "exception.h"
#include "Response.h"
#include "MalformedMessage.h"
    MalformedMessageException::MalformedMessageException(): BaseException("Message is malformed!",ERROR) {}
