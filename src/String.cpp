
#include "String.h"
#include <iostream>
#include <string>
#include "helpers.h"
#include "exception.h"
#include "StringValue.h"
#include "Logger.h"
#include "ValueSet.h"


int8_t String::read(const uint8_t* start, const uint8_t* end, Response& res) {

    uint32_t len = 0;
    if (!Helper::read_int<uint32_t>(start, end, len, 4)) return 0;

    if (!Helper::read_str(start, end, len, command)) return 0;

    uint32_t len_key = 0;
    if (!Helper::read_int<uint32_t>(start, end, len_key, 4)) return 0;
    if (!Helper::read_str(start, end, len_key, key)) return 0;
    enum Str::StringCommand cmd = String::parseCommand(command);
     if((cmd == Str::StringCommand::SET || cmd == Str::StringCommand::CONCAT)){
        if(end == start){
            res.status = ERROR;
            res.output = "Please supply a string for this operation!";
            return -1;
        }
        uint32_t str_len = 0;
        if(!Helper::read_int<uint32_t>(start,end,str_len, 4)) return 0;
         if (!Helper::read_str(start, end, str_len, value)) return 0;
     }
    if (start != end) return 0; // trailing garbage

    return 1;
}


enum Str::StringCommand String::parseCommand(std::string command) {

    if(command == "set"){ return Str::StringCommand::SET; }
    if(command == "get"){ return Str::StringCommand::GET; }
    if(command == "del") {return Str::StringCommand::DEL; }
    if(command == "concat") {return Str::StringCommand::CONCAT; }
    if(command == "len") {return Str::StringCommand::LENGTH; }
    else return Str::StringCommand::UNKNOWN;
}



void String::execute(Storage* storage, Response& res) {
 std::string commandStr = this->command;
    try {
        switch(parseCommand(command)) {
            case Str::StringCommand::SET: {
                Value::set<StringValue, std::string>(key,value);
                res.output = "OK";
                break;
            }

            case Str::StringCommand::GET: {
                Value* val = Value::get(key);
                StringValue* str = dynamic_cast<StringValue*>(val);
                if (!str) throw StorageException("incorrect type for get", ERROR);
                res.output = str->toString();
                break;
            }
             case Str::StringCommand::DEL: {
                Value::del(key);
                res.output = "OK";
                break;
            }

            case Str::StringCommand::LENGTH: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                StringValue* str = dynamic_cast<StringValue*>(val);
                if (!str) throw StorageException("incorrect type for increment", ERROR);

                int len = str->length();
                res.output = std::to_string(len);
                break;
            }

            case Str::StringCommand::CONCAT: {
             Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                StringValue* str = dynamic_cast<StringValue*>(val);
                if (!str) throw StorageException("incorrect type for increment", ERROR);

                str->concat(value);
                res.output = str->toString();
                break;

            }
            default:
                throw StorageException("unknown command", ERROR);
        }

    } catch (const StorageException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + commandStr + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);

}




