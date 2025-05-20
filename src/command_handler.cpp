
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
            
            while(cmd.size() < nstr){
                // read 4 bytes length, then n bytes length of message
                uint32_t len = 0;
                if(!Helper::read_int<uint32_t>(request,end,len,4)){
                   throw MalformedMessageException();
                }
               
            std::string text;
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
        
        if(cmd.size() < 1){
            throw WrongCommandException("Cannot have an empty command!");
        }
        std::string& command = cmd[0];
        if(Type::_parseCommand(command) != Generic::UNKNOWN){
            // generic command
            type = new Type();
        } else if(command == "set") {
            // do set command, only allowed in scalar not lists
            
            
            if(cmd.size() < 3 || cmd.size() > 4){
                throw WrongCommandException("format is set [key] [value] ?[hint]!");
            }
            // enforce that keys cannot be command names or start with numbers and only start with
            Type::isKeyValid(cmd[1]);
            std::string hint = "", &value = cmd[2];
            // here there is a key and value and maybe a hint eys or no
            if(cmd.size() == 4){
                hint = cmd[3];
            }
            NumberKind t = Helper::isNumber(value);
            if(hint == ""){
                // try parsing and create correct type
                if(t == NumberKind::INTEGER){
                    type = new Number<int64_t>();
                } else if(t == NumberKind::DOUBLE){
                    type = new Number<double>();
                } else {
                    type = new String();
                }
            } else {
                // check for type based on hint
                if(hint != "int" && hint != "double" && hint != "string"){
                    throw WrongCommandException("hint is only string, int, or double!");
                    
                }
                if((hint == "int" || hint == "double") && t == NumberKind::NOT_NUMBER ){
                    throw WrongCommandException("type mismatch! Couldn't convert to a number type");
                }

                 if(hint == "int"){
                    type = new Number<int64_t>();
                } else if(hint == "double"){
                    type = new Number<double>();
                } else {
                    type = new String();
                }
                cmd.erase(cmd.begin() + 3);

            }
        } else {
            Storage* storage = Storage::getInstance();
            if(cmd.size() < 2){
                throw WrongCommandException("Cannot have less than 2 strings for command");
            }
            
            Type::isKeyValid(cmd[1]);
            Value** val = storage->table->get(cmd[1]);
            // based on this value create the correct type and pass type to do_response keep all checks in execute
              // start parsing based on command and key
            
            if(val == NULL)  {
                type = new Type();
                if(Type::_parseCommand(cmd[0]) == Generic::UNKNOWN || Number<int>::parseCommand(cmd[0]) == NumberCommand::UNKNOWN || String::parseCommand(cmd[0]) == Str::StringCommand::UNKNOWN ) throw WrongCommandException("Unknown command!");
                throw NotFoundException();
            };
            switch((*val)->getType()){
                case ValueType::DOUBLE:
                    type = new Number<double>();
                    break;

                case ValueType::INT64: // ints
                    type = new Number<int64_t>();
                    break;
                case ValueType::STRING:
                    type = new String();
                    break;
                default:
                    // unsupported type
                    
                     throw WrongCommandException("Unsupported type!");
                    break;
                
            }
        }
        type->read(cmd,res);
        
    }
    bool CommandHandler::handle_request(const uint8_t *request, size_t length, std::vector<uint8_t>& output){
            std::vector<std::string> cmd;
            Response res;
            Type* type = nullptr;
            try{
                
                CommandHandler::read_request(request, length, cmd, res);
                CommandHandler::parse_request(cmd, res, type);

            } 
            catch(const BaseException& e){
                res.output = e.what();
                res.status = e.getResponse();
                
            }
            
            CommandHandler::do_response(type, res, output);

            if(type) delete type;
            return true;

        }


