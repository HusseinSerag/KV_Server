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
        if(command == "len") {return ListC::LEN;}
        if(command == "execute") {return ListC::EXECUTE; }
        if(command == "remove") {return ListC::LREMOVE; }
        if(command == "update") {return ListC::UPDATE; }
        return ListC::UNKNOWN;
    };
  

//     void List::parse_add(std::vector<std::string>& cmd, NumberKind& t){
//    // first make sure there is atleast 3 strings
//                     if(cmd.size() < 3){
//                         throw WrongCommandException("format is ladd [key] ...[values] ?[hint]!");
//                     }
//                     // enforce that keys cannot be command names or start with numbers and only start with
//                     Type::isKeyValid(cmd[1]);
//                     // ladd x 3 234 42 sf int can add several values and a hint 
//                     if(cmd.back() != "string" && cmd.back() != "int" && cmd.back() != "double"){
//                         // no type hint try parsing and check all is correct type
//                         // based on first value type we expect this time for the rest
//                          t = Helper::isNumber(cmd[2]);
//                         // if it is a number then make sure we store numbers too
//                         if(t != NumberKind::NOT_NUMBER){
//                             for(int i = 3; i < cmd.size() ; i++){
//                                 const std::string& m =  t == NumberKind::DOUBLE ? "double" : "integer";
//                                 if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
//                                 NumberKind cand = Helper::isNumber(cmd[i]);
//                                 if(cand != t && cand == NumberKind::NOT_NUMBER){ 
//                                     const std::string& m = (t == NumberKind::DOUBLE ? "double" : "integer");
//                                     throw WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
//                                 }
//                             }
//                         } else {
//                             for(int i = 2; i < cmd.size(); i++){

//                                 if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Format specifier can only be at end of command!");
//                             }
//                         }
//                         // here we have made sure all have the same type and we have the type
                         
//                     } else {
//                         // we have a hint at the end
//                         // make sure no format specifier anywhere else
//                         std::string& hint = cmd.back();
//                         for(int i = 2; i < cmd.size() - 1; i++){
//                                 if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Format specifier can only be at end of command!");
//                         }
//                         t = hint == "int" ? NumberKind::INTEGER : hint == "double" ? NumberKind::DOUBLE : NumberKind::NOT_NUMBER;
//                         // if hint is specified make sure if hint is number then we supply numbers
//                         for(int i = 2; i < cmd.size() - 1; i++){
//                             if(Helper::isNumber(cmd[i]) == NumberKind::NOT_NUMBER && t != NumberKind::NOT_NUMBER ){
//                             throw WrongCommandException("type mismatch! Couldn't convert to a number type");
//                         }
//                     }
//                     cmd.pop_back();
                        
//                         }
// }




//     ladd x 1 2 3 4 string ttl 120
//    capture values from 3rd strings till we find ttl or string
//  ladd x string string
void List::parse_add(std::vector<std::string>& cmd, NumberKind& t){
   // first make sure there is atleast 3 strings
                    if(cmd.size() < 3){
                        throw WrongCommandException("format is ladd [key] ...[values] ?[hint] ?ttl [ttl_seconds]!");
                    }
                    // enforce that keys cannot be command names or start with numbers and only start with
                    Type::isKeyValid(cmd[1]);
                    // ladd x 3 234 42 sf int can add several values and a hint 
                    // go until we find hint or ttl

                    // either at end or before ttl
                    // string ttl 3
                    // ttl 3 string
                    int hint_index = -1;
                    int ttl_index = -1;
                    int config_index = -1;
                    bool valuePassed = false;
                     
                    
                    for(int i = 2; i < cmd.size();i++){

                        if(cmd[i] == "ttl" || cmd[i] == "string" || cmd[i] == "double" || cmd[i] == "int"){
                            config_index = i;
                            break;
                        } else {
                            valuePassed = true;
                        }
                    }
                   if(!valuePassed && config_index != -1){
                    throw WrongCommandException("format is ladd [key] ...[values] ?[hint] ?ttl [ttl_seconds]!");
                   }
                    if(config_index != -1) {
                        for(int i = config_index; i < cmd.size();i++){
                            if(cmd[i] == "string" || cmd[i] == "double" || cmd[i] == "int"){
                                if(hint_index != -1) throw WrongCommandException("Cannot have several type hints at once!");
                            hint_index = i;
                        } else if(cmd[i] == "ttl"){
                             if(i + 1 >= cmd.size()){
                                throw WrongCommandException("format is ladd [key] ...[values] ?[hint] ?ttl [ttl_seconds]!");
                        }
                        ttl_index = i + 1;
                        i++;
                        } else {
                            throw WrongCommandException("hformat is ladd [key] ...[values] ?[hint] ?ttl [ttl_seconds]!");
                        }
                    }
                }
                    
                  
                          if(hint_index == -1){
                        // no type hint try parsing and check all is correct type
                        // based on first value type we expect this time for the rest
                         t = Helper::isNumber(cmd[2]);
                        // if it is a number then make sure we store numbers too
                        int bounds = ttl_index != -1 ? ttl_index : cmd.size();
                        if(t != NumberKind::NOT_NUMBER){
                            for(int i = 3; i < bounds ; i++){
                                const std::string& m =  t == NumberKind::DOUBLE ? "double" : "integer";
                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
                                NumberKind cand = Helper::isNumber(cmd[i]);
                                if(cand != t && cand == NumberKind::NOT_NUMBER){ 
                                    const std::string& m = (t == NumberKind::DOUBLE ? "double" : "integer");
                                    throw WrongCommandException("Cannot store a non number value to a [" + m + "] list!" );
                                }
                            }
                        } else {
                            for(int i = 2; i < bounds; i++){

                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Format specifier can only be at end of command!");
                            }
                        }
                        // here we have made sure all have the same type and we have the type
                         
                    } else {
                        // we have a hint at the end
                        // make sure no format specifier anywhere else
                        std::string& hint = cmd[hint_index];

                        for(int i = 2; i < cmd.size(); i++){
                            if(i == hint_index) continue;
                                if(cmd[i] == "string" || cmd[i] == "int" || cmd[i] == "double") WrongCommandException("Already have a hint!");
                        }
                        t = hint == "int" ? NumberKind::INTEGER : hint == "double" ? NumberKind::DOUBLE : NumberKind::NOT_NUMBER;
                        // if hint is specified make sure if hint is number then we supply numbers
                        for(int i = 2; i < cmd.size(); i++){
                            if(ttl_index != -1){
                                if(i == ttl_index || i == ttl_index - 1) continue;
                            }
                            if(Helper::isNumber(cmd[i]) == NumberKind::NOT_NUMBER && t != NumberKind::NOT_NUMBER ){
                            throw WrongCommandException("type mismatch! Couldn't convert to a number type");
                        }
                    }
                   cmd.erase(cmd.begin() + hint_index);
                        
            }
            if(ttl_index != -1){
                if(Helper::isNumber(cmd[ttl_index]) != NumberKind::INTEGER){
                    throw WrongCommandException("TTL value must be an integer");
                }
            }


            
    }
                



