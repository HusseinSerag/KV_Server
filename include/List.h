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
    template <class L, class T, class V>
     void execute(Storage* storage, Response& res, std::vector<V>& values);

     template <class T>
   void read(std::vector<std::string>& request, std::vector<T>& values,enum ListC::ListCommand cmd);
    
   

};

#include "List.tpp"

