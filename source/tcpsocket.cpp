#include "tcpsocket.h"

#if defined(__unix__) || defined(__APPLE__)
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
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

bool TcpSocket::open(const SockOpt opt){
    const int opt_val = 1;
#if defined(WIN32) || defined(_WIN32)
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)){
        return false;
    }
#endif
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        return false;
    }
    if (opt & ReuseaddrOpt){
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) != 0){
            return false;
        }
    } 
    if (opt & NonblockOpt){
        int flags = fcntl(_socket, F_GETFL, 0);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
    }
    if (opt & NoDelayOpt){
        if (setsockopt(_socket, SOL_SOCKET, TCP_NODELAY, &opt_val, sizeof(opt_val)) != 0){
            return false;
        }
    }
    return true;
}

ssize_t TcpSocket::read(char* dest, size_t size){
    return ::read(_socket, dest, size);
};

ssize_t TcpSocket::send(const char* src, size_t size){
    return ::write(_socket, src, size);
};

bool TcpSocket::close(){
#if defined(__unix__) || defined(__APPLE__)
    return ( (::close(_socket) == 0)? true: false);
#else
    bool ret = (closesocket(_socket))? false: true;
    WSACleanup();
    return ret;
#endif
};

void TcpSocket::flush(){
    static char garbage_sock[512];
#if defined(__unix__) || defined(__APPLE__)
    while (recv(_socket, (void*)garbage_sock, sizeof(garbage_sock), MSG_DONTWAIT) > 0);
#endif
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
    if (!parentSocket.open(ReuseaddrOpt)){
        throw ExcOpenSocket();
    }
    if (parentSocket.bind(hostAddr, hostPort) == false){
        throw ExcBind();
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

