#pragma once
#include "Number.h"
#include <iostream>
#include <string>
#include "helpers.h"
#include "exception.h"
#include "ValueSet.h"
#include "NumberValue.tpp"
#include "Logger.h"

template <typename T>
int8_t Number<T>::read(const uint8_t* start, const uint8_t* end, Response& res) {

    uint32_t len = 0;
    if (!Helper::read_int<uint32_t>(start, end, len, 4)) return 0;

    if (!Helper::read_str(start, end, len, command)) return 0;

    uint32_t len_key = 0;
    if (!Helper::read_int<uint32_t>(start, end, len_key, 4)) return 0;
    if (!Helper::read_str(start, end, len_key, key)) return 0;
    enum NumberCommand cmd = Number::parseCommand(command);
     if((cmd == NumberCommand::SET || cmd == NumberCommand::MULT || cmd == NumberCommand::DIVIDE )){
        if(end - start != sizeof(T)){
            res.status = ERROR;
            res.output = "Please supply a number for this operation!";
            return -1;
        }
        if (!Helper::read_int<T>(start, end, value, sizeof(T))) return 0;
     }


    if (start != end) return 0; // trailing garbage

    return 1;
}

template <typename T>
enum NumberCommand Number<T>::parseCommand(std::string command) {

    if(command == "set"){ return NumberCommand::SET; }
    if(command == "get"){ return NumberCommand::GET; }
    if(command == "mult") { return NumberCommand::MULT; }
    if(command == "dec") { return NumberCommand::DEC ;}
    if(command == "inc") {return NumberCommand::INC; }
    if(command == "del") {return NumberCommand::DEL; }
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

            case NumberCommand::GET: {
                Value* val = Value::get(key);
                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw StorageException("incorrect type for get", ERROR);
                res.output = val->toString();
                break;
            }

            case NumberCommand::INC: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw StorageException("incorrect type for increment", ERROR);

                u64Val->increment();
                res.output = "OK";
                break;
            }

            case NumberCommand::DEC: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw StorageException("incorrect type for decrement", ERROR);

                u64Val->decrement();
                res.output = "OK";
                break;
            }

            case NumberCommand::MULT: {
                Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw StorageException("incorrect type for multiplication", ERROR);

                u64Val->multiply(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::DIVIDE: {
                if (value == 0) throw StorageException("division by zero", ERROR);

                Value* val = storage->table->get(key);
                if (val == NULL) throw StorageException("not found", NOT_FOUND);

                NumberValue<T>* u64Val = dynamic_cast<NumberValue<T>*>(val);
                if (!u64Val) throw StorageException("incorrect type for division", ERROR);

                u64Val->divide(value);
                res.output = "OK";
                break;
            }

            case NumberCommand::DEL: {
                Value::del(key);
                res.output = "OK";
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




