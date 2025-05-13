#pragma once
#include <string>
class Serializable {
    public:
    virtual void save(std::ostream& out) = 0;
    virtual void load(std::istream& in) = 0;
    virtual ~Serializable() = default;

};
