#include "List.h"
#include "StringList.h"
#include "exception/WrongCommandException.h"
#include "ListValue.h"

#include "StringListValue.h"
int8_t StringList::read(std::vector<std::string>& request, Response& res){

    
    if(request.size() < 2){
            throw WrongCommandException("atleast 2 strings are needed for list commands!");
        }
        this->command = request[0];
        this->key = request[1];
        enum ListC::ListCommand cmd = List::parseCommand(this->command);
        if(cmd == ListC::LADD){
            for(int i = 2; i < request.size(); i++){
                this->values.push_back((request[i]));
            }
        }
       _read_common(request, cmd);
        return 1;
    }



    void StringList::execute(Storage* storage, Response& res){
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
                        res.status = ERROR;
                        return;
                        
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
            break;   
        }
        default:
            break;
        }
    }