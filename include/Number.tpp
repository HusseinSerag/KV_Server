
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

template <typename T>
int8_t Number<T>::read(std::vector<std::string> & command, Response& res) {
      
    if(command.size() < 2){
        throw WrongCommandException("atleast a command and key required!" );
    }
    if(command.size() > 3) {// error 
       // throw error
        throw WrongCommandException("invalid command!" );
    }
    this->command = command[0];
    this->key = command[1];
    enum NumberCommand cmd = parseCommand(this->command);
    if(cmd == NumberCommand::DEC || cmd == NumberCommand::INC){
        if(Helper::isNumber(command[2]) == NumberKind::NOT_NUMBER){
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
    try {
        switch(parseCommand(command)) {
            case NumberCommand::SET: {

                Value* val = new NumberValue<T>(value);
                storage->table->set(key, val);

                res.output = "OK";
                break;
            }
            case NumberCommand::INC: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw BaseException("incorrect type for increment", ERROR);

                u64Val->increment();
                res.output = "OK";
                break;
            }

            case NumberCommand::DEC: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw BaseException("incorrect type for decrement", ERROR);

                u64Val->decrement();
                res.output = "OK";
                break;
            }

            case NumberCommand::MULT: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw BaseException("incorrect type for multiplication", ERROR);

                u64Val->multiply(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::DIVIDE: {
                if (value == 0) throw BaseException("division by zero", ERROR);

                Value* val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw BaseException("incorrect type for division", ERROR);

                u64Val->divide(value);
                res.output = "OK";
                break;
            }
            default:
            if(Type::_parseCommand(command) != Generic::UNKNOWN || String::parseCommand(command) !=  Str::StringCommand::UNKNOWN){
                 throw BaseException("Type mismatch: command '"+command+"' is not valid for number",ERROR);
            }
                throw BaseException("unknown command", ERROR);
        }

    } catch (const BaseException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + commandStr + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);
}




