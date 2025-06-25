#include "Type.h"
#include "Connection.h"
#include "datastore.h"
#include "Response.h"
#include <string>
#include <iostream>
#include "Value.h"
#include "helpers.h"
#include <deque>
#include "exception/exception.h"
#include "Logger.h"
#include "Number.h"
#include "String.h"
#include "exception/TypeMismatchException.h"
#include "exception/WrongCommandException.h"
#include "List.h"

Type::~Type() {}
Type::Type():ttl(-1) {}


void Type::write(std::vector<uint8_t>& out_buffer, Response& res) {
    uint32_t len = 4 + res.output.size();
    Connection::appendToBuffer(out_buffer, (const uint8_t*)&len, 4);
    Connection::appendToBuffer(out_buffer, (const uint8_t*)&res.status, 4);
    Connection::appendToBuffer(out_buffer, (const uint8_t*)res.output.data(), res.output.size());
}

int8_t Type::read(std::vector<std::string> & command, Response& res){
    this->command = command[0];
    enum Generic::GenericCommands cmd = _parseCommand(this->command);
    if(cmd == Generic::GET || cmd == Generic::DEL){
        
        if(command.size() != 2){
             throw WrongCommandException("format is " + this->command + " [key]" );
        }
        Type::isKeyValid(command[1]);
        this->key = command[1];
    } else if(cmd != Generic::UNKNOWN) {
        if(command.size() != 1){
            throw WrongCommandException("format is '" + this->command +"' only!");
        }
    }
    
    
    return 1;

}

 enum Generic::GenericCommands Type::_parseCommand(const std::string& command) {

    if(command == "size" ) return Generic::SIZE;
    if(command == "capacity") return Generic::CAPACITY;
    if(command == "keys") return Generic::KEYS;
    if(command == "table") return Generic::TABLE;
    if(command == "get") return Generic::GET;
    if(command == "del") return Generic::DEL;
    return Generic::UNKNOWN;

 }

 void Type::execute(Storage* storage, Response& res){
 std::string commandStr = this->command;
    try{
    switch(_parseCommand(command)){
            
        case Generic::GET: {
                Value* val = Value::get(key);
                res.output = val->toString();
                break;
            }
             case Generic::DEL: {
                Value::del(key);
                res.output = "OK";
                break;
            }
        case Generic::KEYS: {
            std::deque<std::string> keys = storage->table->keys();
            if(keys.size() == 0) throw BaseException("No keys exist!",ERROR);

            res.output = Helper::deque_to_string(keys);
            break;
        }
        case Generic::SIZE: {
            res.output = std::to_string(storage->table->getSize());
            break;
        }
        case Generic::CAPACITY: {
            res.output = std::to_string(storage->table->getCapacity());
            break;
        }
        case Generic::TABLE: {
            res.output = storage->table->table_toString();
            break;
        }
        case Generic::UNKNOWN:
        default:
        if(Number<int64_t>::parseCommand(command) != NumberCommand::UNKNOWN || String::parseCommand(command) != Str::StringCommand::UNKNOWN || List::parseCommand(command) != ListC::UNKNOWN){
                throw TypeMismatchException(command, "generic commands");
            }
        throw WrongCommandException("unknown command");
    }
    } catch (const BaseException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
       
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + commandStr + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);

 }



void Type::isKeyValid(const std::string& key){
    // check if key not name of any command and doesn't start with number or any special character but _
    
    if(Type::_parseCommand(key) != Generic::UNKNOWN || Number<int>::parseCommand(key) != NumberCommand::UNKNOWN || String::parseCommand(key) != Str::StringCommand::UNKNOWN ){
        throw WrongCommandException("Cannot have key with name of a command!");
        
    }
    if(key[0] >= 'a' && key[0] <= 'z' || key[0] >= 'A' && key[0] <= 'Z' || key[0] == '_'){
        return;
    }
    throw WrongCommandException("key must start with an alphabetical letter or underscore!");
 }


 std::string& Type::getCommand(){
    return this->command;
 }

void Type::parse_set(std::vector<std::string>& cmd, NumberKind& t) {
       // do set command, only allowed in scalar not lists
            // set key value [hint] ttl [ttl_value]
            if(cmd.size() < 3 || cmd.size() > 6){
                throw WrongCommandException("format is set [key] [value] ?[hint] ?[ttl [ttl_value]]!");
            }
            int ttl_index = -1;
            int hint_index = -1;
           
            // enforce that keys cannot be command names or start with numbers and only start with
            Type::isKeyValid(cmd[1]);
            if(cmd[2] == "int" || cmd[2] == "double" || cmd[2] == "string" || cmd[2] == "ttl"){
                throw WrongCommandException("format is set [key] [value] ?[hint] ?[ttl [ttl_value]]!");
            }
            for(int i = 3; i < cmd.size();i++){
                if(cmd[i] == "int" || cmd[i] == "double" || cmd[i] == "string"){
                    hint_index = i;
                } else if(cmd[i] == "ttl"){
                    if(i + 1 >= cmd.size()){
                                    throw WrongCommandException("format is set [key] [value] ?[hint] ?[ttl [ttl_value]]!");
                            if(Helper::isNumber(cmd[i + 1]) != NumberKind::INTEGER){
                                throw WrongCommandException("ttl value must be an integer!");
                            }
                        }
                        ttl_index = i + 1;
                        i++;
                } else {
                         throw WrongCommandException("format is set [key] [value] ?[hint] ?[ttl [ttl_value]]!");
                }
            }
            std::string &value = cmd[2];
            // here there is a key and value and maybe a hint yes or no
            
             t = Helper::isNumber(value);
            if(hint_index != -1){
                // try parsing and create correct type
                // check for type based on hint
               std::string& hint = cmd[hint_index];
                if(hint != "int" && hint != "double" && hint != "string"){
                    throw WrongCommandException("hint is only string, int, or double!");
                    
                }
                if((hint == "int" || hint == "double") && t == NumberKind::NOT_NUMBER ){
                    throw WrongCommandException("type mismatch! Couldn't convert to a number type");
                }

                 if(hint == "int"){
                    t = NumberKind::INTEGER;
                } else if(hint == "double"){
                    t = NumberKind::DOUBLE;
                } else {
                    t = NumberKind::NOT_NUMBER;
                }
                cmd.erase(cmd.begin() + hint_index);
            }
 }

//  void Type::parse_set(std::vector<std::string>& cmd, NumberKind& t) {
//        // do set command, only allowed in scalar not lists
//             // set key value [hint] ttl [ttl_value]
//             if(cmd.size() < 3 || cmd.size() > 4){
//                 throw WrongCommandException("format is set [key] [value] ?[hint]!");
//             }
//             // enforce that keys cannot be command names or start with numbers and only start with
//             Type::isKeyValid(cmd[1]);
//             std::string hint = "", &value = cmd[2];
//             // here there is a key and value and maybe a hint yes or no
//             if(cmd.size() == 4){
//                 hint = cmd[3];
//             }
//              t = Helper::isNumber(value);
//             if(!(hint == "")){
//                 // try parsing and create correct type
//                 // check for type based on hint
//                 if(hint != "int" && hint != "double" && hint != "string"){
//                     throw WrongCommandException("hint is only string, int, or double!");
                    
//                 }
//                 if((hint == "int" || hint == "double") && t == NumberKind::NOT_NUMBER ){
//                     throw WrongCommandException("type mismatch! Couldn't convert to a number type");
//                 }

//                  if(hint == "int"){
//                     t = NumberKind::INTEGER;
//                 } else if(hint == "double"){
//                     t = NumberKind::DOUBLE;
//                 } else {
//                     t = NumberKind::NOT_NUMBER;
//                 }
//                 cmd.erase(cmd.begin() + 3);
//             }
//  }