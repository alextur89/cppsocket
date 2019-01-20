#pragma once

#include "abstractsocket.h"
#include <vector>

namespace cppsocket{
namespace udp{
    class UdpSocket: public AbstractSocket{
            socket_t _socket;        
            addr_t _addr;        
        public:
            UdpSocket();
            bool open(const std::string addr, unsigned port, const SockOpt = EmptyFlag)override;
            unsigned read(char* dest, size_t size)override;
            unsigned send(const char* src, size_t size)override;
            bool close()override;
            void flush()override;
            socket_t getsocket() const;
            ~UdpSocket();
    };
    class UdpClient{
        private:
            std::vector<UdpSocket> _sockets;
        public:
            UdpClient() = default;
            void append(const UdpSocket& sock);
            std::vector<UdpSocket> ready(long microsec);
    };
}
}
