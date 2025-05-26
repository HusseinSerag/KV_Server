#pragma once
#include "Type.h"

namespace ListC {

    enum ListCommand {
        LADD, LEN, EXECUTE, LREMOVE, UPDATE, UNKNOWN
    };
}

class List : public Type {
    
    protected:
    int index;
    bool hasIndex;
    void  _read_common(std::vector<std::string>& request, ListC::ListCommand cmd);
    public:
    static void parse_add(std::vector<std::string>& cmd, NumberKind& t);
    static enum ListC::ListCommand parseCommand(const std::string& command);
    //virtual int8_t read(std::vector<std::string>& request, Response& res);
   // virtual void execute(Storage* storage, Response& res);
    static void parse(std::vector<std::string>& cmd, Type* &type);
    
   

};

