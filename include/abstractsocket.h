#pragma once

#include <arpa/inet.h>

namespace cppsocket{
    typedef unsigned SockOpt;
    enum SockFlags{
        Reuseaddr = 0x1,
        Nonblock = 0x2,
        Broadcast = 0x4,
        Bind = 0x8
    };
    class ISocket{
        protected:
            virtual unsigned read(char* dest, size_t size) = 0;
            virtual unsigned send(const char* src, size_t size) = 0;
            virtual bool open(std::string addr, unsigned port, const SockOpt = 0) = 0;
            virtual bool close() = 0;
            virtual void flush() = 0;   
    };
};
