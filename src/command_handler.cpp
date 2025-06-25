
#include "command_handler.h"
#include "Connection.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include "datastore.h"
#include "Type.h"
#include "Number.h"
#include "Response.h"
#include "helpers.h"
#include "String.h"
#include "exception/exception.h"
#include "exception/NotFoundException.h"
#include "exception/MalformedMessage.h"
#include "List.h"
#include "StringList.h"
#include "NumberList.h"


void CommandHandler::writeResponse(std::vector<uint8_t>& output, const std::string& data, Response& out) {
 // write length of response which is status plus size of data
    uint32_t resp_length = 4 + data.size();
    Connection::appendToBuffer(output, (const uint8_t *)&resp_length, 4);
    Connection::appendToBuffer(output,(const uint8_t *)&out.status,4);
    Connection::appendToBuffer(output, (const uint8_t*)data.data(), data.size());

}


 void CommandHandler::do_response(Type* type, Response &res, std::vector<uint8_t>& output) {
    Storage* storage = Storage::getInstance();
    bool hasExecuted = false;
    if(res.status == OK){
        hasExecuted = true;
        type->execute(storage, res);
    }
    if(!hasExecuted){
        std::string logMessage = Helper::getCurrentTimestamp() +" | Status: " + (res.status == OK ? "Success" : "Error") + (res.status != OK ? " | Reason: " + res.output : "");
    Logger::log(logMessage);
    }

    if(res.status == OK) storage->write();
    Type::write(output, res);
}


  int CommandHandler::read_request(const uint8_t* request, size_t length,  std::vector<std::string>& cmd, Response& res){
           
        
            const uint8_t *end = request + length;
            // first read type
            uint32_t nstr = 0;
            
            if(!Helper::read_int<uint32_t>(request,end,nstr,4)){
              throw  MalformedMessageException(); // replace with malformed message exception
            }
            
            // read entire request sent
            cmd.reserve(nstr);
            while(cmd.size() < nstr){
                // read 4 bytes length, then n bytes length of message
                uint32_t len = 0;
                if(!Helper::read_int<uint32_t>(request,end,len,4)){
                   throw MalformedMessageException();
                }
               
            std::string text;
            text.reserve(len);
            if(!Helper::read_str(request,end,len, text)){
               throw MalformedMessageException();
            }
            cmd.push_back(text);
         }
        
         if (request != end) throw MalformedMessageException(); // trailing garbage
         
       
        return 0;
    
    }
    void CommandHandler::parse_request(std::vector<std::string>& cmd, Response& res, Type* & type) {
    
       
        // all in try catch block
        // start parsing based on command and key
        // normal set and del and get 
        
        if(cmd.empty()){
            throw WrongCommandException("Cannot have an empty command!");
        }
        std::string& command = cmd[0];
        if(Type::_parseCommand(command) != Generic::UNKNOWN) {
            // generic command
            type = new Type();
        } else if(command == "set" || command == "ladd") {
            // setter commands treated differently
            NumberKind t;
            if(command == "set"){
                Type::parse_set(cmd,t);
                switch(t) {
                    case NumberKind::INTEGER:
                        type = new Number<int64_t>();
                        break;
                    case NumberKind::DOUBLE:
                        type = new Number<double>();
                        break;
                    case NumberKind::NOT_NUMBER:
                        type = new String();
                        break;

                }
            } else {
                List::parse_add(cmd,t);
                switch(t) {
                    case NumberKind::INTEGER:
                        type = new NumberList<int64_t>();
                        break;
                    case NumberKind::DOUBLE:
                        type = new NumberList<double>();
                        break;
                    case NumberKind::NOT_NUMBER:
                        type = new StringList();
                        break;
                }
            }
         
        } else {
            Storage* storage = Storage::getInstance();
            if(cmd.size() < 2){
                throw WrongCommandException("Cannot have less than 2 strings for command");
            }
            Type::isKeyValid(cmd[1]);
            Value** val = storage->get(cmd[1]);
            // based on this value create the correct type and pass type to do_response keep all checks in execute
              // start parsing based on command and key
            
            if(val == NULL)  {
                type = new Type();
                if(Type::_parseCommand(command) == Generic::UNKNOWN && Number<int>::parseCommand(command) == NumberCommand::UNKNOWN && String::parseCommand(command) == Str::StringCommand::UNKNOWN && List::parseCommand(command) == ListC::UNKNOWN) throw WrongCommandException("Unknown command!");
                throw NotFoundException();
            };
            switch((*val)->getType()){
                case ValueType::DOUBLE:
                    type = new Number<double>("0",*val);
                    break;
                case ValueType::INT64: // ints
                    type = new Number<int64_t>("0",*val);
                    break;
                case ValueType::STRING:
                    type = new String("",*val);
                    break;
                case ValueType::LIST_STRING:
                    type = new StringList(*val);
                    break;
                case ValueType::LIST_DOUBLE:
                    type = new NumberList<double>(*val);
                    break;
                case ValueType::LIST_INT64:
                    type = new NumberList<int64_t>(*val);
                    break;
                default:
                    // unsupported type
                     throw WrongCommandException("Unsupported type!");
                    break;
                
            }
        }
        
    }
    bool CommandHandler::handle_request(const uint8_t *request, size_t length, std::vector<uint8_t>& output){
            std::vector<std::string> cmd;
            Response res;
            Type* type = nullptr;
            try{
                
                CommandHandler::read_request(request, length, cmd, res);
                CommandHandler::parse_request(cmd, res, type);
                type->read(cmd,res);

            } 
            catch(const BaseException& e){
                res.output = e.what();
                res.status = e.getResponse();

                if(type){ delete type; type = nullptr; }
                
            }
            
            CommandHandler::do_response(type, res, output);

            if(type) delete type;
            return true;

        }


