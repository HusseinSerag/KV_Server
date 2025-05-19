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


Type::~Type() {}
Type::Type() {}


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
             throw BaseException("format is " + this->command + " [key]",ERROR );
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
        if(Number<int64_t>::parseCommand(command) != NumberCommand::UNKNOWN || String::parseCommand(command) != Str::StringCommand::UNKNOWN){
                throw BaseException("Type mismatch: command '"+command+"' is not valid for generic commands",ERROR);
            }
            
                throw BaseException("unknown command", ERROR);
        throw BaseException("unknown command", ERROR);
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
        throw BaseException("Cannot have key with name of a command!",ERROR);
        
    }
    if(key[0] >= 'a' && key[0] <= 'z' || key[0] >= 'A' && key[0] <= 'Z' || key[0] == '_'){
        return;
    }
    throw BaseException("key must start with an alphabetical letter or underscore!",ERROR);
 }