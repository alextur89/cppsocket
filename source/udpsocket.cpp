#include "udpsocket.h"

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "exception.h"
#include <algorithm>
#include <memory>

namespace cppsocket{
namespace udp{

UdpSocket::UdpSocket(){
    _socket = -1;
}

UdpSocket::UdpSocket(const UdpSocket& the){
    _socket = the._socket;
    _addr = the._addr;
}

UdpSocket::~UdpSocket(){
    close();
}

socket_t UdpSocket::getsocket() const{
    return _socket;
}

bool UdpSocket::open(const std::string addr, unsigned port, const SockOpt opt){
    const int opt_val = 1;
    if (addr.empty()){
        return false;
    }
    if ((_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        throw ExcOpenSocket();
    }
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(addr.c_str());
    _addr.sin_port = htons(port);

    if (opt & SockFlags::Reuseaddr){
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) != 0){
            throw ExcSetSockOpt();
        }
    } 
    if (opt & SockFlags::Broadcast){
        if (setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &opt_val, sizeof(opt_val)) != 0){
            throw ExcSetSockOpt();
        }
    } 
    if (opt & SockFlags::Bind){
        if (bind(_socket, (struct sockaddr*) &_addr, sizeof(struct sockaddr_in)) == -1){
            throw ExcBind();
        }
    }
    if (opt & SockFlags::Nonblock){
        int flags = fcntl(_socket, F_GETFL, 0);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
    }
    return true;
}

unsigned UdpSocket::read(char* dest, size_t size){
    return recv(_socket, dest, size, 0);
};

unsigned UdpSocket::send(const char* src, size_t size){
    return sendto(_socket, src, size, 0, (struct sockaddr*) &_addr, sizeof(_addr));
};

bool UdpSocket::close(){
    return ( (::close(_socket) == 0)? true: false);
};

void UdpSocket::flush(){
    static char garbage_sock[512];
    while (recv(_socket, (void*)garbage_sock, sizeof(garbage_sock), MSG_DONTWAIT) > 0);
};

void UdpClient::append(std::shared_ptr<UdpSocket> sock){
    _sockets.push_back(sock);
}

std::vector<std::shared_ptr<UdpSocket> > UdpClient::ready(long microsec){
    std::vector<std::shared_ptr<UdpSocket> > result;
    if (_sockets.empty()){
        throw ExcEmptySocketContainer();
    }
    int max = (*std::max_element(std::begin(_sockets), std::end(_sockets), 
            [](std::shared_ptr<UdpSocket> sk1, std::shared_ptr<UdpSocket> sk2){
            return (sk1->getsocket() > sk2->getsocket());
            }))->getsocket();

    if (max < 0){
        throw ExcOpenSocket();
    }

    fd_set set;
    for (auto s: _sockets){
        FD_SET(s->getsocket(), &set);
    }
    std::shared_ptr<struct timeval> t;
    if (microsec >= 0){
        t = std::make_shared<struct timeval>();
        constexpr long p = 1e+6;
        t->tv_sec = microsec / p;
        t->tv_usec = microsec % p;
    }
    if (select(max + 1, &set, NULL, NULL, t.get()) > 0){
        for (auto s: _sockets){
            if (FD_ISSET(s->getsocket(), &set)){
                result.push_back(s);
            }
        }
    }
    return result;
}

}
}

