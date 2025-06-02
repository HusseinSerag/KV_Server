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
        enum ListC::ListCommand cmd = List::parseCommand(request[0]);
        List::read(request,values,cmd);  
 
        return 1;
    }



    void StringList::execute(Storage* storage, Response& res){
        std::string& cmd = this->command;
        try{
        switch (List::parseCommand(this->command))
        {
        // case ListC::LADD:{
        //         StringListValue* list = NULL;
        //        if(val == NULL){
        //         // no list so override
        //          list = new StringListValue();
        //         storage->table->set(key,list);
        //        } else {

        //         ValueType t = val->getType();
        //           // if list then we know its a list
        //            if(!Value::isListType(t)){
        //             // this isnt a list we can override 
        //                 list = new StringListValue();
        //                 storage->table->set(key,list);
        //            } else {
        //             // list with maybe wrong type so we check
                   
        //             if(!val->getType() != ValueType::LIST_STRING){
        //                 // error
        //                 throw TypeMismatchException(this->command, "list of strings");  
        //             } else {
        //                 list = (StringListValue* )val;
        //             }
        //            }
        //         }
        //         // dynamic cast
        //         for(int i = 0; i < values.size(); i++){
        //            //insert
        //            ListValue::insert<StringValue *>(new StringValue(values[i]), list->getValue());
        //         }
        //         res.output = "OK";
        //     break;   
        // }
        case ListC::EXECUTE:
        break;
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
    