#include "tcpsocket.h"

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
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
namespace tcp{

TcpSocket::TcpSocket(){
    _socket = -1;
    _is_connected = false;
}

TcpSocket::TcpSocket(const TcpSocket& the){
    _socket = the._socket;
    _addr = the._addr;
    _server_addr = the._server_addr;
    _is_connected = the._is_connected;
}

TcpSocket::~TcpSocket(){
    if (_socket != -1){
        close();
    }
}

socket_t TcpSocket::getsocket() const{
    return _socket;
}

bool TcpSocket::open(const SockOpt opt, const std::string addr, unsigned port){
    const int opt_val = 1;
    if (addr.empty()){
        return false;
    }
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
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
    if (opt & SockFlags::Bind){
        if (bind(_socket, (struct sockaddr*) &_addr, sizeof(struct sockaddr_in)) == -1){
            throw ExcBind();
        }
    }
    if (opt & SockFlags::Nonblock){
        int flags = fcntl(_socket, F_GETFL, 0);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
    }
    if (opt & SockFlags::NoDelay){
        if (setsockopt(_socket, SOL_SOCKET, TCP_NODELAY, &opt_val, sizeof(opt_val)) != 0){
            throw ExcSetSockOpt();
        }
    }
    return true;
}

unsigned TcpSocket::read(char* dest, size_t size){
    if (!_is_connected){
        throw ExcNotConnected();
    }
    return ::read(_socket, dest, size);
};

unsigned TcpSocket::send(const char* src, size_t size){
    if (!_is_connected){
        throw ExcNotConnected();
    }
    return ::write(_socket, src, size);
};

bool TcpSocket::close(){
    return ( (::close(_socket) == 0)? true: false);
};

void TcpSocket::flush(){
    static char garbage_sock[512];
    while (recv(_socket, (void*)garbage_sock, sizeof(garbage_sock), MSG_DONTWAIT) > 0);
};

bool TcpSocket::connect(const std::string addr, unsigned port){
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    _server_addr.sin_port = htons(port);
    if (::connect(_socket, (struct sockaddr*) &_server_addr, sizeof(_server_addr)) != 0){
        _is_connected = false;
    }
    else{
        _is_connected = true;
    }
    return _is_connected;
};

TcpServer::TcpServer(const std::string hostAddr, unsigned hostPort){
    if (!parentSocket.open(Bind | Reuseaddr, hostAddr,  hostPort)){
        throw ExcOpenSocket();
    }
}

bool TcpServer::listen(unsigned countOfConn){
    if (::listen(parentSocket.getsocket(), countOfConn) < 0){
        return false;
    }
    return true;
}

bool TcpServer::accept(std::function<int(TcpSocket&)> handler, int& result){
    int addrlen = sizeof(addr_t);
    addr_t clientAddr;
    auto newSock = ::accept(parentSocket.getsocket(), (struct sockaddr*) &clientAddr, (socklen_t*) &addrlen);
    if (newSock < 0){
        return false;
    }
    TcpSocket clientSocket{newSock, clientAddr, parentSocket._addr, true};
    result = handler(clientSocket);
    return true;
}

bool TcpServer::close(){
    parentSocket.close();
    return true;
}

TcpServer::~TcpServer(){
    close();
}

}
}

