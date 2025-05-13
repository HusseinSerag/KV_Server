#pragma once

#include <stdint.h>
#include <unistd.h>
#include <vector>

class Connection {
public:
    explicit Connection(int fd);
    ~Connection();

    int get_fd() const;
    bool wants_read() const;
    bool wants_write() const;
    bool wants_close() const;

    void set_want_read(bool val);
    void set_want_write(bool val);
    void set_want_close(bool val);

    void handle_read();
    void handle_write();

static void appendToBuffer(std::vector<uint8_t> &buf, const uint8_t *data, size_t len) ;

 static void consumeFromBuffer(std::vector<uint8_t> &buf, size_t n) ;

private:
    int fd_;
    bool want_read_;
    bool want_write_;
    bool want_close_;

    std::vector<uint8_t> incoming_;
    std::vector<uint8_t> outgoing_;

    bool try_one_request();
};
