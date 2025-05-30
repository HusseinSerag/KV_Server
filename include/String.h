
#pragma once

#include "Type.h"
#include "Scalar.h"
#include <iostream>
#include "Value.h"
namespace Str {
 enum StringCommand {
    SET,  CONCAT, LENGTH, UNKNOWN,
};
}
class String : public Scalar {

    std::string value;
public:
    int8_t read(std::vector<std::string> &request, Response& res) override;
    void execute(Storage* storage, Response& res) override;
    static enum Str::StringCommand parseCommand(const std::string& command);
    String();
    String(const std::string& val,Value* value = NULL);
    
    friend std::ostream& operator<<(std::ostream& os, const String& str);
    Value* toValue() override;
    std::string getValue();

};


