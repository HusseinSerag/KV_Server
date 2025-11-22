
#pragma once
#include <string>
#include <vector>
#include "Type.h"
#include "Response.h"
#include "ConcurrentQueue.h"



class CommandHandler {
   static ConcurrentQueue* queue;

public:
    
static void setQueue(ConcurrentQueue* q) {
  queue = q;
}
static void writeResponse(std::vector<uint8_t>& output, const std::string& data, Response& out);


static void do_response(Type* type, Response &out, std::vector<uint8_t>& output);

static void parse_request(std::vector<std::string>& cmd, Response& res,Type* & type);
 
    static int read_request(const uint8_t* request, size_t length,  std::vector<std::string>& cmd, Response& res);


    static bool handle_request(const uint8_t *request, size_t length, std::vector<uint8_t>& output);

};
