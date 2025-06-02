#include "NumberList.h"
#include "Value.h"
#include "Response.h"
#include "datastore.h"
#include "NumberListValue.h"

template <typename T>
NumberList<T>::NumberList(Value* val){
    this->val = val;
}

template <typename T>
int8_t NumberList<T>::read(std::vector<std::string>& request, Response& res) {
  if(request.size() < 2){
            throw WrongCommandException("atleast 2 strings are needed for list commands!");
        }
        enum ListC::ListCommand c = List::parseCommand(request[0]);
        List::read(request,values,c);  
 
        return 1;
}

template <typename T>
void NumberList<T>::execute(Storage* storage, Response& res) {
        std::string& cmd = this->command;
        try{
        switch (List::parseCommand(this->command))
        {
        
            case ListC::EXECUTE:
            break;
            default:
            List::execute<NumberListValue<T>,NumberValue<T>,double>(storage,res,this->values);
        }
  
    } catch(const BaseException& exp) {
        res.output = exp.what();
        res.status = exp.getResponse();
    }
    std::string logMessage = Helper::getCurrentTimestamp() + " Command: \"" + this->command + "\" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);
}
