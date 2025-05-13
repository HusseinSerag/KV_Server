
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




void CommandHandler::writeResponse(std::vector<uint8_t>& output, const std::string& data, Response& out) {
 // write length of response which is status plus size of data
    uint32_t resp_length = 4 + data.size();
    Connection::appendToBuffer(output, (const uint8_t *)&resp_length, 4);
    Connection::appendToBuffer(output,(const uint8_t *)&out.status,4);
    Connection::appendToBuffer(output, (const uint8_t*)data.data(), data.size());

}


 void CommandHandler::do_response(Type* type, Response &res, std::vector<uint8_t>& output) {
    Storage* storage = Storage::getInstance();
    if(res.status != ERROR)
    type->execute(storage, res);

    if(res.status == OK) storage->write();
    type->write(output, res);
}


  int CommandHandler::parse_request(const uint8_t* request, size_t length,  Type* &toWork, Response& res){
        // get number of strings
         const uint8_t *end = request + length;
         // first read type
         uint16_t type = 0;

         if(!Helper::read_int<uint16_t>(request,end,type,2)){
            return -1;
         }


         // after reading type we parse based on type
        switch(type){
            case 0x00: // generic commands
                toWork = new Type();
                break;
            case 0x01:
                toWork = new Number<double>();
                break;

            case 0x02: // ints
                toWork = new Number<int64_t>();
                break;
            case 0x03:
                toWork = new String();
                break;
            default:
                // unsupported type
                res.output = "Unsupported type!";
                res.status = ERROR;
                break;
                return 0;
        }
        if(toWork->read(request, end,res) == 0) return -1;
        return 0;
    }


    bool CommandHandler::handle_request(const uint8_t *request, size_t length, std::vector<uint8_t>& output){
            Type* type = NULL;
            Response res;
            if(CommandHandler::parse_request(request, length, type, res) < 0){
                return false; // error
            }
            if(type == NULL){
                return false;
            }
            CommandHandler::do_response(type, res, output);

            delete type;
            return true;

        }


