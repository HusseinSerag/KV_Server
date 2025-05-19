
#pragma once

#include "Type.h"

namespace Str {
 enum StringCommand {
    SET,  CONCAT, LENGTH, UNKNOWN,
};
}
class String : public Type {

    std::string value;
public:
    int8_t read(std::vector<std::string> &request, Response& res) override;
    void execute(Storage* storage, Response& res) override;
    static enum Str::StringCommand parseCommand(const std::string& command);

};


