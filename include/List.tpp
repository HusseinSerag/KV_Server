
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

template<class>
struct always_false:std::false_type {};

template <class T>
T parseValue(const std::string& input){
    if constexpr (std::is_same_v<T,std::string>){
        return input;
    } else if constexpr (std::is_same_v<T,double>){
        return std::stod(input);
    } else {
        static_assert(always_false<T>::value,"Unsupported type");
    }
}
template <class T>
void List::read(std::vector<std::string>& request, std::vector<T>& values,enum ListC::ListCommand cmd){
        this->command = request[0];
        this->key = request[1];
        hasIndex = false;
        if(cmd == ListC::LADD){
            for(int i = 2; i < request.size(); i++){
                values.push_back(parseValue<T>(request[i]));
            }
        }
         else if(cmd == ListC::LEN){
        if(request.size() > 2){
            throw WrongCommandException("format is len [key] !");
        }
    }
    else if(cmd == ListC::LREMOVE) {
        if(request.size() < 3 || request.size() > 4){
            throw WrongCommandException("format is remove [key] ([value] | index [index])!");
        }
        if(request[2] == "index"){
            if( Helper::isNumber(request[3]) == NumberKind::NOT_NUMBER ||  Helper::isNumber(request[3]) == NumberKind::DOUBLE){
            throw WrongCommandException("Index value must be an integer");
        }
        if(request.size() == 3){
            throw WrongCommandException("please provide an index value!");
        }
        index = std::stoi(request[3]);
        hasIndex = true;
    } else {
        if(request.size() == 4){
              throw WrongCommandException("format is remove [key] [value]");
        }
        values.push_back(parseValue<T>(request[2]));
    }
        
    } else if(cmd == ListC::UPDATE){
        // update key value [new value]
        // update key index number [new value]
        if(request.size() < 4 || request.size() > 5){
              throw WrongCommandException("format is update [key] ([value] [new value] | index [index] [new value])!");
        }
        if(request[2] == "index"){
            // expect index
            if( Helper::isNumber(request[3]) == NumberKind::NOT_NUMBER ||  Helper::isNumber(request[3]) == NumberKind::DOUBLE){
            throw WrongCommandException("Index value must be an integer");
        }
        if(request.size() == 4){
            throw WrongCommandException("format is update [key] index [index] [new value]");
        }
        index = std::stoi(request[3]);
        hasIndex = true;
        values.push_back(parseValue<T>(request[4]));

        }
         else {
                if(request.size() == 5){
              throw WrongCommandException("format is update [key] [value] [new value]");
        }
        values.push_back(parseValue<T>(request[2]));
        values.push_back(parseValue<T>(request[3]));
        }
    }
    }


 template <class L, class T, class V>
 void List::execute(Storage* storage, Response& res, std::vector<V>& values) {
        std::string& command = this->command;
        enum ListC::ListCommand cmd = parseCommand(command);
        L* list = NULL;
        if(val != NULL){

            ValueType t = val->getType();
            if(Value::isListType(t)){
                list = (L *)this->val;
            } else {
                throw TypeMismatchException(this->command, "list");
            }
        }
        switch(cmd){
             case ListC::LADD:{
               Value** v = storage->get(this->key);
               if(v == NULL){
                // no list so override
                list = new L();
                storage->table->set(key,list);
               } else {
                val = *v;
                ValueType t = (val)->getType();
                  // if list then we know its a list
                   if(!Value::isListType(t)){
                    // this isnt a list we can override 
                        list = new L();
                        storage->table->set(key,list);
                   } else {
                    // list with maybe wrong type so we check
                    L l;
                    if((val)->getType() != l.getType()){
                        // error
                        throw TypeMismatchException(this->command, "wrong type");  
                    } else {
                        list = (L* )(val);
                    }
                   }
                }
                // dynamic cast
                for(int i = 0; i < values.size(); i++){
                   //insert
                   ListValue::insert<T *>(new T(values[i]), list->getValue());
                }
                res.output = "OK";
            break;   
        }
     
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
                   
                    list->updateAtIndex(new T(values[0]), index + 1, list->getValue());
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

