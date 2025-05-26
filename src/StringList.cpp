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
        try{
        std::string& cmd = this->command;
        switch (List::parseCommand(this->command))
        {
        case ListC::LADD:{

            Value** val = storage->table->get(this->key);
              StringListValue* list = NULL;
               if(val == NULL){
                // no list so override
                 list = new StringListValue();
                storage->table->set(key,list);
               } else {

                  ListValue* l = dynamic_cast<ListValue*>(*val);
                  // if list then we know its a list
                   if(!l){
                    // this isnt a list we can override 
                        list = new StringListValue();
                        storage->table->set(key,list);
                   } else {
                    // list with maybe wrong type so we check
                    StringListValue* ll = dynamic_cast<StringListValue*>(l);
                    if(!ll){
                        // error
                        throw TypeMismatchException(this->command, "list of strings");
                        
                    } else {
                        list = ll;
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
        
        case ListC::LEN: {
                Value** val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                ListValue* list = dynamic_cast<ListValue*>(*val);
                if (!list) throw BaseException("incorrect type for llen", ERROR);
                int len = list->length();
                res.output = std::to_string(len);
                break;
        }
        case ListC::LREMOVE: {
             Value** val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                StringListValue* list = dynamic_cast<StringListValue*>(*val);
                if (!list) throw BaseException("incorrect type for remove", ERROR);
                if(!hasIndex){
                    
                    StringValue toSearch(values[0]);
                    if(list->getValue()->search(&toSearch) == list->getValue()->getNil()){
                        throw NotFoundException();
                    } 
                    list->getValue()->remove(&toSearch);
                    
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
             Value** val = storage->table->get(key);
                if (val == NULL) throw NotFoundException();

                StringListValue* list = dynamic_cast<StringListValue*>(*val);
                if (!list) throw BaseException("incorrect type for update", ERROR);
                if(!hasIndex){
                     StringValue toSearch(values[0]);
                     
                    if(list->getValue()->search(&toSearch) == list->getValue()->getNil()){
                        throw NotFoundException();
                    } 
                    list->getValue()->remove(&toSearch);
                    ListValue::insert<StringValue *>(new StringValue(values[1]), list->getValue());
                    
                } else {
                     if(index + 1 > list->length() || index < 0){
                        throw BaseException(("Index must be between 0 and " + std::to_string(list->length() - 1)),ERROR);
                    }
                   
                    list->updateAtIndex(new StringValue(values[0]), 1, list->getValue());
                }
                res.output = "OK";
                break;

        }
        // add case update by element then save to memory commands then implement number types then refactor
        default:
            if(Number<int64_t>::parseCommand(command) != NumberCommand::UNKNOWN || Type::_parseCommand(command) != Generic::UNKNOWN || String::parseCommand(command) != Str::StringCommand::UNKNOWN){
                throw TypeMismatchException(command,"list of strings");
            }
            
                throw WrongCommandException("unknown command");
        }
  
    } catch(const BaseException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + this->command + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);

  
    }