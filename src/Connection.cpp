
#include "Connection.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Server.h"
#include "helpers.h"
#include <string>
#include <map>
#include "command_handler.h"




Connection::Connection(int fd)
    : fd_(fd), want_read_(true), want_write_(false), want_close_(false) {}

Connection::~Connection() {
    close(fd_);
}

int Connection::get_fd() const { return fd_; }
bool Connection::wants_read() const { return want_read_; }
bool Connection::wants_write() const { return want_write_; }
bool Connection::wants_close() const { return want_close_; }

void Connection::set_want_read(bool val) { want_read_ = val; }
void Connection::set_want_write(bool val) { want_write_ = val; }
void Connection::set_want_close(bool val) { want_close_ = val; }

void
Connection::appendToBuffer(std::vector<uint8_t> &buf, const uint8_t *data, size_t len) {
    buf.insert(buf.end(), data, data + len);
}


 void Connection::consumeFromBuffer(std::vector<uint8_t> &buf, size_t n) {
    buf.erase(buf.begin(), buf.begin() + n);
}
void Connection::handle_write() {
    ssize_t rv = write(fd_, &outgoing_[0], outgoing_.size());
    if (rv < 0 && errno == EAGAIN) return;
    if (rv < 0) {
        Helper::msg_errno("write() error");
        want_close_ = true;
        return;
    }

    consumeFromBuffer(outgoing_,(size_t)rv);
    if (outgoing_.size() == 0) {
        want_read_ = true;
        want_write_ = false;
    }
}

void Connection::handle_read() {
    uint8_t buf[64 * 1024]; // 64KB
    ssize_t rv = read(fd_, buf, sizeof(buf));
    if (rv < 0 && errno == EAGAIN) return;
    if (rv < 0) {
        Helper::msg_errno("read() error");
        want_close_ = true;
        return;
    }

    if (rv == 0) {
     //   Helper::msg(incoming_.size() == 0 ? "client closed" : "unexpected EOF");
        want_close_ = true;
        return;
    }

    appendToBuffer(incoming_, buf, rv);

    while (try_one_request()) {}

    if (outgoing_.size() > 0) {
        want_read_ = false;
        want_write_ = true;
        handle_write();
    }
}




bool Connection::try_one_request() {
    if (incoming_.size() < 4) return false;

    uint32_t len = 0;

    memcpy(&len, incoming_.data(), 4);

    if (len > Server::k_max_msg) {
        Helper::msg("message too long");
        want_close_ = true;
        return false;
    }

    if (4 + len > incoming_.size()) return false;

    const uint8_t *request = &incoming_[4];

    if(!CommandHandler::handle_request(request, len, outgoing_)){
        want_close_ = true;
        return false; // error
    }

    // application logic done! remove the request message.
    consumeFromBuffer(incoming_, 4 + len);

    return true;
}




