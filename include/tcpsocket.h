#pragma once

/*!
 * \file
 * \brief TcpSocket header file
 *
 */

#include "abstractsocket.h"
#include <list>
#include <functional>
#include <memory>

namespace cppsocket{
namespace tcp{
    /*!
    *    \class TcpSocket
    *    \brief The TcpSocket class which implement a tcp client
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class TcpSocket: public AbstractSocket{
            friend class TcpServer;
            addr_t _server_addr;        
            bool _is_connected;
            ssize_t send(const char* src, size_t size, std::string addr, unsigned port) override{ return 0;}
            TcpSocket(socket_t s, addr_t a, addr_t sa, bool is_connected = true):
                _server_addr(sa),
                _is_connected(is_connected){
                    _socket = s;
                    _addr = a;
                }
        public:
            TcpSocket();
            TcpSocket(const TcpSocket&);
            TcpSocket(TcpSocket&&) = delete;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            *    \return true if successfull, false otherwise  
            */
            bool open(const SockOpt = EmptyFlagOpt)override;
            /*!
            *    Connect to host 
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return true if successfull, false otherwise  
            */
            bool connect(const std::string addr = "127.0.0.1", unsigned port = 0);
            /*!
            *    Read from socket
            *    \param[out] dest Pointer to destination buffer
            *    \param[in] size Function shall attempt read size bytes from socket
            *    \return Upon successful completion the function shall return number of bytes read
            */
            ssize_t read(char* dest, size_t size)override;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            ssize_t send(const char* src, size_t size)override;
            /*!
            *    Close socket
            *    \return true if successfull, false otherwise  
            */
            bool close()override;
            /*!
            *    Flush buffered socket data
            */
            void flush()override;
            /*!
            *    Function shall return sockets descriptor 
            */
            socket_t getsocket() const;
            ~TcpSocket();
    };
    /*!
    *    \class TcpServer
    *    \brief The TcpServer class which implement a tcp server
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class TcpServer{
            TcpSocket parentSocket;
        public:
            TcpServer(const std::string hostAddr, unsigned hostPort);
            TcpServer() = delete;
            TcpServer(const TcpSocket&) = delete;
            TcpServer(TcpSocket&&) = delete;
            /*!
            *    Make this socket ready to accept connection requests
            *    \param[in] countOfConn Count of requests to queue up
            *    \return true if successfull, false otherwise  
            */
            bool listen(unsigned countOfConn = 5);
            /*!
            *    Wait for a connection request
            *    param[in] handler A function for handle
            *    param[in] result A result of handle function
            *    \return true if successfull, false otherwise  
            */
            bool accept(std::function<int(TcpSocket&)> handler, int& result);
            /*!
            *    Close server
            *    \return true if successfull, false otherwise  
            */
            bool close();
            ~TcpServer();
    };
}
}
