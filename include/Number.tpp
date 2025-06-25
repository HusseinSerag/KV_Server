
#include "Number.h"
#include <iostream>
#include <string>
#include "helpers.h"
#include "exception/exception.h"
#include "ValueSet.h"
#include "NumberValue.h"
#include "Logger.h"
#include "Type.h"
#include "String.h"
#include "exception/NotFoundException.h"
#include "exception/WrongCommandException.h"
#include "exception/TypeMismatchException.h"
#include "List.h"

template <typename T>
Number<T>::Number(){
    this->val = NULL;
}

template <typename T>
Number<T>::Number(const std::string& value,Value* val){
    this->value = std::stod(value);
    this->val = val;
}
template <typename T>
int8_t Number<T>::read(std::vector<std::string> & command, Response& res) {
      
    if(command.size() < 2){
        throw WrongCommandException("atleast a command and key required!" );
    }
     this->command = command[0];
    this->key = command[1];
    enum NumberCommand cmd = parseCommand(this->command);
    if(command.size() > 3 && cmd != NumberCommand::SET) {// error 
       // throw error
       
        throw WrongCommandException("invalid command!");
    }
   
    if(cmd == NumberCommand::DEC || cmd == NumberCommand::INC){
        if(command.size() == 3 && Helper::isNumber(command[2]) == NumberKind::NOT_NUMBER){
             throw WrongCommandException("Value provided must be a number!");
        }
        this->value = command.size() == 3 ? std::stod(command[2]) : 1;
    }
     else if(command.size() < 3 && (cmd == NumberCommand::SET || cmd == NumberCommand::MULT || cmd == NumberCommand::DIVIDE )){
         throw WrongCommandException("format is " + this->command + " [key] [value] " + ((this->command == "set") ? "?[hint]" : ""));
    } 
    if( cmd == NumberCommand::SET || cmd == NumberCommand::MULT || cmd == NumberCommand::DIVIDE ){
        if(Helper::isNumber(command[2]) == NumberKind::NOT_NUMBER){
             throw WrongCommandException("Value provided must be a number!" );
        }
        this->value = std::stod(command[2]);
    }

    if(cmd == NumberCommand::SET){
        if(command.size() == 5)
        this->ttl = std::stoi(command[4]);
   }
    
    
    return 1;
}

template <typename T>
enum NumberCommand Number<T>::parseCommand(const std::string& command) {

   if(command == "set") {return NumberCommand::SET;}
    if(command == "mult") { return NumberCommand::MULT; }
    if(command == "dec") { return NumberCommand::DEC ;}
    if(command == "inc") {return NumberCommand::INC; }
    if(command == "div") {return NumberCommand::DIVIDE; }
    else return NumberCommand::UNKNOWN;
}


template <typename T>
void Number<T>::execute(Storage* storage, Response& res) {
 std::string commandStr = this->command;
    NumberValue<T>* num = NULL;
    try {

        if(this->val != NULL){
            if(val->getType() != ValueType::DOUBLE && val->getType() != ValueType::INT64){
                throw BaseException("incorrect type for number operation!", ERROR);
            }
            num = (NumberValue<T>*)(val);
        }
        switch(parseCommand(command)) {
            case NumberCommand::SET: {

                Value* val = new NumberValue<T>(value);
                storage->set(key, val,this->ttl);

                res.output = "OK";
                break;
            }
            case NumberCommand::INC: {
                num->increment(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::DEC: {
                num->decrement(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::MULT: {
                num->multiply(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::DIVIDE: {
                if (value == 0) throw BaseException("division by zero", ERROR);
                num->divide(value);
                res.output = "OK";
                break;
            }
            default:
            if(Type::_parseCommand(command) != Generic::UNKNOWN || String::parseCommand(command) !=  Str::StringCommand::UNKNOWN || List::parseCommand(command) != ListC::UNKNOWN)  {
                 throw TypeMismatchException(command, "number");
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


template <typename T>
double Number<T>::getValue() const{
    return value;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Number<T>& n) {
    os << "value:" << n.getValue() << std::endl;
    return os;
}
template <typename T>
Value* Number<T>::toValue() {
    return new NumberValue<T>(this->value);
}