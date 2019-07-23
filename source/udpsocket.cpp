#include "udpsocket.h"

#include <stdio.h>
#include <unistd.h>
#if defined(linux) || defined(__APPLE__)
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
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
    if (_socket != -1){
        close();
    }
}

socket_t UdpSocket::getsocket() const{
    return _socket;
}

bool UdpSocket::open(const SockOpt opt, const std::string addr, unsigned port){
    const int opt_val = 1;
<<<<<<< HEAD
=======
    if (addr.empty()){
        return false;
    }
#if defined(WIN32) || defined(_WIN32)
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)){
        throw ExcOpenSocket();
    }
#endif
>>>>>>> windows_support
    if ((_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        throw ExcOpenSocket();
    }

    if (opt & ReuseaddrOpt){
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) != 0){
            throw ExcSetSockOpt();
        }
    } 
    if (opt & BroadcastOpt){
        if (setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &opt_val, sizeof(opt_val)) != 0){
            throw ExcSetSockOpt();
        }
    } 
    if (opt & BindOpt){
        if (!AbstractSocket::bind(addr, port)){
            throw ExcBind();
        }
    }
<<<<<<< HEAD
    if (opt & NonblockOpt){
=======
    if (opt & SockFlags::Nonblock){
#if defined(linux) || defined(__APPLE__)
>>>>>>> windows_support
        int flags = fcntl(_socket, F_GETFL, 0);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
#endif
    }
    return true;
}

unsigned UdpSocket::read(char* dest, size_t size){
    return recv(_socket, dest, size, 0);
};

unsigned UdpSocket::send(const char* src, size_t size, std::string addr, unsigned port){
    std::unique_ptr<addr_t> the_addr(new addr_t);
    the_addr->sin_family = AF_INET;
    the_addr->sin_addr.s_addr = inet_addr(addr.c_str());
    the_addr->sin_port = htons(port);
    return sendto(_socket, src, size, 0, (struct sockaddr*) the_addr.get(), sizeof(addr_t));
};

unsigned UdpSocket::send(const char* src, size_t size){
    return sendto(_socket, src, size, 0, (struct sockaddr*) &_addr, sizeof(_addr));
};

bool UdpSocket::close(){
#if defined(linux) || defined(__APPLE__)
    return ( (::close(_socket) == 0)? true: false);
#else
    bool ret = (closesocket(_socket))? false: true;
    WSACleanup();
    return ret;
#endif
};

void UdpSocket::flush(){
    static char garbage_sock[512];
#if defined(linux) || defined(__APPLE__)
    while (recv(_socket, (void*)garbage_sock, sizeof(garbage_sock), MSG_DONTWAIT) > 0);
#endif
};

void UdpSelect::append(const UdpSocket& sock){
    _sockets.push_back(std::make_shared<UdpSocket>(sock));
}

void UdpSelect::ready(std::list<std::shared_ptr<UdpSocket> >& result, long microsec){
    if (_sockets.empty()){
        throw ExcEmptySocketContainer();
    }
#if defined(linux) || defined(__APPLE__)
    int max = (*std::max_element(std::begin(_sockets), std::end(_sockets), 
            [](std::shared_ptr<UdpSocket> sk1, std::shared_ptr<UdpSocket> sk2){
            return (sk1->getsocket() > sk2->getsocket());
            }))->getsocket();

    if (max < 0){
        throw ExcOpenSocket();
    }
#else
    int max = 0;
#endif

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
}

}
}

