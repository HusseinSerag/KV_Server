#pragma once
#include "Type.h"
#include "Value.h"
class Scalar: public Type {


public:
virtual Value* toValue() = 0;
};