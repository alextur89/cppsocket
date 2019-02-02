#pragma once

#include "abstractsocket.h"
#include <list>
#include <memory>

namespace cppsocket{
namespace udp{
    class UdpSocket: public AbstractSocket{
            socket_t _socket;        
            addr_t _addr;        
        public:
            UdpSocket();
            UdpSocket(const UdpSocket&);
            bool open(const SockOpt = EmptyFlag, const std::string addr = "127.0.0.1", unsigned port = 0)override;
            unsigned read(char* dest, size_t size)override;
            unsigned send(const char* src, size_t size)override;
            unsigned send(const char* src, size_t size, std::string addr, unsigned port)override;
            bool close()override;
            void flush()override;
            socket_t getsocket() const;
            ~UdpSocket();
    };
    class UdpClient{
        private:
            std::list<UdpSocket> _sockets;
        public:
            enum class Wait: long{
                infinity = -1,
                immediately = 0
            };
            UdpClient() = default;
            void append(const UdpSocket& sock);
            void append(std::initializer_list<UdpSocket> list);
            void ready(std::list<UdpSocket>& result, long microsec = static_cast<long>(Wait::infinity));
    };
}
}
