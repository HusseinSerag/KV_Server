#pragma once
#include <vector>
#include "Connection.h"
#include <poll.h>
class Server {
    int accepting_fd;
    // fds take smallest possible non negative number, so a dynamic array will be better
    // try to use a map or something better
    std::vector<Connection *> connections;
    std::vector<struct pollfd> poll_fds;
    Connection* handle_accept(int fd);
    public:
     Server();
     void configure(int port);
     void start();
     static void fd_set_nb(int fd);
    static const size_t k_max_msg;
    friend class Connection;





};
