#include "Type.h"
#include "List.h"
#include "exception/WrongCommandException.h"
#include "String.h"
#include "Number.h"
#include "ListValue.h"
#include "StringValue.h"
#include "NumberValue.h"
#include "StringList.h"
#include "datastore.h"


    
     enum ListC::ListCommand List::parseCommand(const std::string& command){
        if(command == "ladd"){return ListC::LADD;}
        if(command == "llen") {return ListC::LEN;}
        if(command == "execute") {return ListC::EXECUTE; }
        if(command == "remove") {return ListC::LREMOVE; }
        if(command == "update") {return ListC::UPDATE; }
        return ListC::UNKNOWN;
    };
    // int8_t List::read(std::vector<std::string>& request, Response& res) {
    //     if(request.size() < 2){
    //         throw WrongCommandException("atleast 2 strings are needed for list commands!");
    //     }
    //     this->command = request[0];
    //     this->key = request[1];
    //     enum ListC::ListCommand cmd = parseCommand(this->command);
        
    //     return 1;
        
    // };

    
   
    // void List::execute(Storage* storage, Response& res) {
    //     std::string& command = this->command;
    //     switch(parseCommand(command)){
    //         case ListC::LEN:
    //              Value** val = storage->table->get(key);
    //             if (val == NULL) throw NotFoundException();

    //             ListValue* list = dynamic_cast<ListValue*>(*val);
    //             if (!list) throw BaseException("incorrect type for llen", ERROR);

    //             int len = length(list);
    //             res.output = std::to_string(len);
    //             break;
                 
    //     }
    // }


void List::parse_add(std::vector<std::string>& cmd, NumberKind& t){
   // first make sure there is atleast 3 strings
                    if(cmd.size() < 3){
                        throw WrongCommandException("format is ladd [key] ...[values] ?[hint]!");
                    }
                    // enforce that keys cannot be command names or start with numbers and only start with
                    Type::isKeyValid(cmd[1]);
                    // ladd x 3 234 42 sf int can add several values and a hint 
                    if(cmd.back() != "string" && cmd.back() != "int" && cmd.back() != "double"){
                        // no type hint try parsing and check all is correct type
                        // based on first value type we expect this time for the rest
                         t = Helper::isNumber(cmd[2]);
                        // if it is a number then make sure we store numbers too
                        if(t != NumberKind::NOT_NUMBER){
                            for(int i = 3; i < cmd.size() ; i++){
                                const std::string& m =  t == NumberKind::DOUBLE ? "double" : "integer";
                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
                                NumberKind cand = Helper::isNumber(cmd[i]);
                                if(cand != t && cand == NumberKind::NOT_NUMBER){ 
                                    const std::string& m = (t == NumberKind::DOUBLE ? "double" : "integer");
                                    throw WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
                                }
                            }
                        } else {
                            for(int i = 2; i < cmd.size(); i++){

                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Format specifier can only be at end of command!");
                            }
                        }
                        // here we have made sure all have the same type and we have the type
                         
                    } else {
                        // we have a hint at the end
                        // make sure no format specifier anywhere else
                        std::string& hint = cmd.back();
                        for(int i = 2; i < cmd.size() - 1; i++){
                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Format specifier can only be at end of command!");
                        }
                        t = hint == "int" ? NumberKind::INTEGER : hint == "double" ? NumberKind::DOUBLE : NumberKind::NOT_NUMBER;
                        // if hint is specified make sure if hint is number then we supply numbers
                        for(int i = 2; i < cmd.size() - 1; i++){
                            if(Helper::isNumber(cmd[i]) == NumberKind::NOT_NUMBER && t != NumberKind::NOT_NUMBER ){
                            throw WrongCommandException("type mismatch! Couldn't convert to a number type");
                        }
                    }
                    cmd.pop_back();
                        
                        }
}

void List::parse(std::vector<std::string>& cmd, Type* &type) {
                 
                enum ListC::ListCommand command = List::parseCommand(cmd[0]);
                NumberKind t;
                if(command == ListC::LADD){
                    List::parse_add(cmd,t);
                }
                    switch(t){
                            case NumberKind::INTEGER:
                           // type = new List<Number<int64_t>,NumberValue<int64_t> *>();
                            break;
                            case NumberKind::DOUBLE:
                           // type = new List<Number<double>,NumberValue<double>*>();
                            break;
                            case NumberKind::NOT_NUMBER:
                            type = new StringList();
                        }
                }
            



void List::_read_common(std::vector<std::string>& request,  ListC::ListCommand cmd){
  
}