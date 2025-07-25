
#include "String.h"
#include <iostream>
#include <string>
#include "helpers.h"
#include "exception/exception.h"
#include "StringValue.h"
#include "Logger.h"
#include "ValueSet.h"
#include "Number.h"
#include "exception/WrongCommandException.h"
#include "exception/NotFoundException.h"
#include "exception/TypeMismatchException.h"

String::String(){
    this->val = NULL;
}
String::String(const std::string& str,Value* val){
    this->value = str;
    this->val = val;

}
int8_t String::read(std::vector<std::string> &request, Response& res)  {

   if(request.size() < 2){
        throw WrongCommandException("atleast a command and key required!" );
    }
    this->command = request[0];
   this->key = request[1];
    enum Str::StringCommand cmd = parseCommand(this->command);
    
    if(request.size() > 3 && cmd != Str::StringCommand::SET){
        throw WrongCommandException("Invalid command!");
    }
   
   if((cmd == Str::StringCommand::SET || cmd == Str::StringCommand::CONCAT ) && request.size() == 2){
      throw WrongCommandException("format is " + this->command + " [key] [value] " + ((this->command == "set") ? "?[hint] ?ttl [ttl_value]" : "") );
   } 
   if(cmd != Str::StringCommand::LENGTH && cmd != Str::StringCommand::UNKNOWN)
    this->value = request[2];

   if(cmd == Str::StringCommand::SET){
        if(request.size() == 5){
            this->ttl = std::stoi(request[4]);
        }
   }
   
   return 1;
}


enum Str::StringCommand String::parseCommand(const std::string& command) {

    if(command == "set"){ return Str::StringCommand::SET; }
    if(command == "concat") {return Str::StringCommand::CONCAT; }
    if(command == "len") {return Str::StringCommand::LENGTH; }
    else return Str::StringCommand::UNKNOWN;
}



void String::execute(Storage* storage, Response& res) {
 std::string& commandStr = this->command;
 StringValue* str = NULL;
 try {
        if(val){
            if( val->getType() != ValueType::STRING){
                throw BaseException("incorrect type for string operation!", ERROR);
            }
            str =(StringValue *)(val);
        }
    
        switch(parseCommand(command)) {
            case Str::StringCommand::SET: {
                Value::set<StringValue, std::string>(key,value,this->ttl);
                res.output = "OK";
                break;
            }
            case Str::StringCommand::LENGTH: {
                int len = str->length();
                res.output = std::to_string(len);
                break;
            }

            case Str::StringCommand::CONCAT: {
                str->concat(value);
                res.output = str->toString();
                break;

            }
            default:
            // check if command is wrong type or not
            if(Number<int64_t>::parseCommand(command) != NumberCommand::UNKNOWN || Type::_parseCommand(command) != Generic::UNKNOWN || List::parseCommand(command) != ListC::UNKNOWN){
                throw TypeMismatchException(command, "string");
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


std::ostream& operator<<(std::ostream& os, const String& str) {
    os << "value:" << str.value << std::endl;
    return os;
}



Value* String::toValue() {
    return new StringValue(this->value);
}

std::string String::getValue() {
    return value;
}