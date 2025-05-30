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

    StringList::StringList(Value* val){
        this->val = val;
    }
int8_t StringList::read(std::vector<std::string>& request, Response& res){

    
    if(request.size() < 2){
            throw WrongCommandException("atleast 2 strings are needed for list commands!");
        }
        this->command = request[0];
        this->key = request[1];
        hasIndex = false;
        enum ListC::ListCommand cmd = List::parseCommand(this->command);
        if(cmd == ListC::LADD){
            for(int i = 2; i < request.size(); i++){
                this->values.push_back((request[i]));
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
        values.push_back(request[2]);
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
        this->values.push_back(request[4]);

        }
         else {
                 if(request.size() == 5){
              throw WrongCommandException("format is update [key] [value] [new value]");
        }
        values.push_back(request[2]);
        values.push_back(request[3]);
        }
    }
        return 1;
    }



    void StringList::execute(Storage* storage, Response& res){
        std::string& cmd = this->command;
        try{
        switch (List::parseCommand(this->command))
        {
        case ListC::LADD:{
                StringListValue* list = NULL;
               if(val == NULL){
                // no list so override
                 list = new StringListValue();
                storage->table->set(key,list);
               } else {

                ValueType t = val->getType();
                  // if list then we know its a list
                   if(!Value::isListType(t)){
                    // this isnt a list we can override 
                        list = new StringListValue();
                        storage->table->set(key,list);
                   } else {
                    // list with maybe wrong type so we check
                   
                    if(!val->getType() != ValueType::LIST_STRING){
                        // error
                        throw TypeMismatchException(this->command, "list of strings");  
                    } else {
                        list = (StringListValue* )val;
                    }
                   }
                }
                // dynamic cast
                for(int i = 0; i < values.size(); i++){
                   //insert
                   ListValue::insert<StringValue *>(new StringValue(values[i]), list->getValue());
                }
                res.output = "OK";
            break;   
        }
     
        default:
            List::execute<StringListValue,StringValue,std::string>(storage,res,values);
        }
  
    } catch(const BaseException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + this->command + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);

  
    }
    