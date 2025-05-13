#include "Server.h"
#include "Connection.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <cstdio>
#include "helpers.h"
#include <cerrno>

    const size_t Server::k_max_msg = 32 << 20;
    Server::Server() {
        this->accepting_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(this->accepting_fd < 0){
            Helper::die("socket()");
        }
        this->connections = {};
        this->poll_fds = {};

    }
    void Server::configure(int port) {
        int val = 1;
        setsockopt(this->accepting_fd,SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = ntohs(port);
        addr.sin_addr.s_addr = ntohl(0);

        int rv = bind(this->accepting_fd, (const sockaddr*)&addr, sizeof(addr));

        if(rv){
            Helper::die("bind()");
        }

        Server::fd_set_nb(accepting_fd);

        rv = listen(accepting_fd, SOMAXCONN);
        if(rv){
            Helper::die("listen()");
        }
    }

    void Server::start() {
     while(true) {
        //prepare the fds that will poll
        this->poll_fds.clear();

        // append accepting fd first
        struct pollfd accepting_fd_obj = {accepting_fd, POLLIN, 0};
        poll_fds.push_back(accepting_fd_obj);

        // append the rest of connections
        for(Connection *conn: connections){

            if(!conn){
                continue;
            }
            struct pollfd conn_fd = {conn->get_fd(), POLLERR, 0};
            if(conn->wants_read()) {
                conn_fd.events |= POLLIN;
            }
            if(conn->wants_write()){
                conn_fd.events |= POLLOUT;
            }
            poll_fds.push_back(conn_fd);
        }

        int rv = poll(poll_fds.data(), (nfds_t) poll_fds.size(), -1);
        if(rv < 0 && errno == EINTR){
            continue;
        }
        if(rv < 0){
            Helper::die("poll");
        }

        if(poll_fds[0].revents) {
            if(Connection *conn = handle_accept(this->accepting_fd)){
                if(connections.size() <= (size_t)conn->get_fd()){
                    connections.resize(conn->get_fd() + 1);
                }
                connections[conn->get_fd()] = conn;
            }
        }
        for(size_t i = 1; i < poll_fds.size() ; i++) {
            int ready = poll_fds[i].revents;
            if(ready == 0) continue;

            Connection* connection = connections[poll_fds[i].fd];
            if(ready & POLLIN){
                connection->handle_read();
            }
            if(ready & POLLOUT){
               connection->handle_write();
            }

            if((ready & POLLERR) || connection->wants_close()){
                connections[connection->get_fd()] = NULL;
                delete connection;
            }
        }

    }

    close(accepting_fd);

    }

    Connection* Server::handle_accept(int fd){

    struct sockaddr_in client = {};
    socklen_t addrlen = sizeof(client);
    int connfd = accept(fd,(struct sockaddr *)&client, &addrlen);
    if(connfd < 0){
        Helper::msg_errno("accept() error");
        return NULL;
    }
    Server::fd_set_nb(connfd);
    uint32_t ip = client.sin_addr.s_addr;
    fprintf(stdout, "new client connected from %u.%u.%u.%u:%u\n",
    ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, (ip >> 24),ntohs(client.sin_port));
    Connection *connection = new Connection(connfd);
    return connection;

}

 void Server::fd_set_nb(int fd) {
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);
    if (errno) {
        Helper::die("fcntl error");
        return;
    }

    flags |= O_NONBLOCK;

    errno = 0;
    (void)fcntl(fd, F_SETFL, flags);
    if (errno) {
        Helper::die("fcntl error");
    }
}
