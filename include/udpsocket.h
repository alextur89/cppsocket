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
            std::list<std::shared_ptr<UdpSocket> > _sockets;
        public:
            UdpClient() = default;
            void append(std::shared_ptr<UdpSocket> sock);
            void append(std::initializer_list<std::shared_ptr<UdpSocket> > list);
            void ready(std::list<std::shared_ptr<UdpSocket> >& result, long microsec = -1);
    };
}
}
