#include "Type.h"
#include "Connection.h"
#include "datastore.h"
#include "Response.h"
#include <string>
#include <iostream>
#include "Value.h"
#include "helpers.h"
#include <deque>
#include "exception.h"
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
    if(this-> command == "get" || this->command == "del"){
        // TODO: IF COMMAND LESS THAN 2
        if(command.size() != 2){
             throw StorageException("format is " + this->command + " [key]",ERROR );
        }
        this->key = command[1];
    }
    
    return 1;

}

 enum Generic::GenericCommands Type::_parseCommand(std::string& command) {

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
            if(keys.size() == 0) throw StorageException("No keys exist!",ERROR);

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
                throw StorageException("Type mismatch: command '"+command+"' is not valid for generic commands",ERROR);
            }
            
                throw StorageException("unknown command", ERROR);
        throw StorageException("unknown command", ERROR);
    }
    } catch (const StorageException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
       
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + commandStr + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);

 }

