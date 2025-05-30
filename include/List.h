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
    public:
    static void parse_add(std::vector<std::string>& cmd, NumberKind& t);
    static enum ListC::ListCommand parseCommand(const std::string& command);
    //virtual int8_t read(std::vector<std::string>& request, Response& res);
   
    template <class L, class T, class V>
     void execute(Storage* storage, Response& res, std::vector<V>& values);
    static void parse(std::vector<std::string>& cmd, Type* &type);
    
   

};

#include "List.tpp"

