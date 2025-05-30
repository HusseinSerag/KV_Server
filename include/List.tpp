
#include "List.h"
#include "StringList.h"
#include "exception/WrongCommandException.h"
#include "ListValue.h"
#include "exception/TypeMismatchException.h"
#include "StringListValue.h"
#include "Number.h"
#include "Value.h"
#include "String.h"
#include "Logger.h"
#include "exception/NotFoundException.h"
 template <class L, class T, class V>
 void List::execute(Storage* storage, Response& res, std::vector<V>& values) {
        std::string& command = this->command;
        L* list = NULL;
        ValueType t = val->getType();
        if(Value::isListType(t)){
           list = (L *)this->val;
        } else {
            throw TypeMismatchException(this->command, "list");
        }
        switch(parseCommand(command)){
                case ListC::LEN: {
                int len = list->length();
                res.output = std::to_string(len);
                break;
        }
        case ListC::LREMOVE: {
                if(!hasIndex){
                    
                    T toSearch(values[0]);
                    if(!list->getValue()->remove(&toSearch)){
                        throw BaseException("Value not found!",ERROR);
                    } 
                } else {
                      
                    if(index + 1 > list->length() || index < 0){
                        throw BaseException(("Index must be between 0 and " + std::to_string(list->length() - 1)),ERROR);
                    }
                    list->removeAtIndex(index + 1, list->getValue());
                }
                res.output = "OK";
                break;

        }
        case ListC::UPDATE: {
                if(!hasIndex){
                     T toSearch(values[0]);
                     
                    if(!list->getValue()->remove(&toSearch)){
                        throw BaseException("Value not found!",ERROR);
                    } 
                    ListValue::insert<T *>(new T(values[1]), list->getValue());
                    
                } else {
                     if(index + 1 > list->length() || index < 0){
                        throw BaseException(("Index must be between 0 and " + std::to_string(list->length() - 1)),ERROR);
                    }
                   
                    list->updateAtIndex(new T(values[0]), 1, list->getValue());
                }
                res.output = "OK";
                break;

        }
         default:
            if(Number<int64_t>::parseCommand(command) != NumberCommand::UNKNOWN || Type::_parseCommand(command) != Generic::UNKNOWN || String::parseCommand(command) != Str::StringCommand::UNKNOWN){
                throw TypeMismatchException(command,"list");
            }
            
                throw WrongCommandException("unknown command");
        
                 
        }
        return;
    }

