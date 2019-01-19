#pragma once

#include <arpa/inet.h>
#include <string>

namespace cppsocket{
    typedef unsigned SockOpt;
    typedef int socket_t;
    typedef struct sockaddr_in addr_t;

    enum SockFlags{
        EmptyFlag = 0,
        Reuseaddr = 0x1,
        Nonblock = 0x2,
        Broadcast = 0x4,
        Bind = 0x8
    };
    class AbstractSocket{
        protected:
            virtual unsigned read(char* dest, size_t size) = 0;
            virtual unsigned send(const char* src, size_t size) = 0;
            virtual bool open(const std::string addr, unsigned port, const SockOpt = 0) = 0;
            virtual bool close() = 0;
            virtual void flush() = 0;   
    };
};
