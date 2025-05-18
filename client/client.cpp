#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string>
#include <vector>
#include <iostream>
#include "helpers.h"

const size_t k_max_msg = 4096;

static int32_t send_req(int fd, std::vector<std::string> &cmd) {
if(cmd.size() < 1) return -1;
    // request parsing
    // several types
    // int64, doubles, strings, lists of ints doubles strings, .....
    // how do we know the type based on request if request starts with apropriate
        uint32_t len = 4;
        for(const std::string c: cmd){
            len += 4 + c.size();
        }
        if (len > k_max_msg) {
        return -1;
    }
    char wbuf[4 + k_max_msg];
    int32_t nstr = cmd.size();
    memcpy(&wbuf[0], &len, 4);  // assume little endian
    memcpy(&wbuf[4],&nstr,4);

    size_t cur = 8;
    for (const std::string &s : cmd) {
        uint32_t p = (uint32_t)s.size();
        memcpy(&wbuf[cur], &p, 4);
        memcpy(&wbuf[cur + 4], s.data(), s.size());
        cur += 4 + s.size();
    }
    return Helper::write_all(fd, wbuf, 4+len);

    return -1;
}

static int32_t read_res(int fd) {
    // 4 bytes header
    char rbuf[4 + k_max_msg + 1];
    errno = 0;
    int32_t err = Helper::read_full(fd, rbuf, 4);
    if (err) {
        if (errno == 0) {
            Helper::msg("EOF");
        } else {
            Helper::msg("read() error");
        }
        return err;
    }

    uint32_t len = 0;
    memcpy(&len, rbuf, 4);  // assume little endian
    if (len > k_max_msg) {
        Helper::msg("too long");
        return -1;
    }

    // reply body
    err = Helper::read_full(fd, &rbuf[4], len);
    if (err) {
        Helper::msg("read() error");
        return err;
    }

    // print the result
    uint32_t rescode = 0;
    if (len < 4) {
        Helper::msg("bad response");
        return -1;
    }
    memcpy(&rescode, &rbuf[4], 4);
    printf("server says: [%u] %.*s\n", rescode, len - 4, &rbuf[8]);
    return 0;
}

int main(int argc, char **argv) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        Helper::die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(3000);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        Helper::die("connect");
    }

    std::vector<std::string> cmd;
    for (int i = 1; i < argc; ++i) {
        cmd.push_back(argv[i]);
    }

    int32_t err = send_req(fd, cmd);
    if (err) {
        return 0;

    }

    err = read_res(fd);
    if (err) {
        return 0;
    }



}
