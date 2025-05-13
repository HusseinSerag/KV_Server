
#pragma once
#include <string>
#include <vector>
#include "Type.h"
#include "Response.h"



class CommandHandler {
public:
static void writeResponse(std::vector<uint8_t>& output, const std::string& data, Response& out);


static void do_response(Type* type, Response &out, std::vector<uint8_t>& output);
static bool read_u32(const uint8_t* &start, const uint8_t *end, uint32_t& out);
 static bool read_str(const uint8_t* &cur, const uint8_t *end, size_t n, std::string &out) ;
    static    int parse_request(const uint8_t* request, size_t length, Type* &toWork, Response& res);


    static bool handle_request(const uint8_t *request, size_t length, std::vector<uint8_t>& output);

};
