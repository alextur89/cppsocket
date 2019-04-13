#pragma once

/*!
 * \file
 * \brief TcpSocket header file
 *
 */

#include "abstractsocket.h"
#include <list>
#include <memory>

namespace cppsocket{
namespace tcp{
    /*!
    *    \class TcpClient
    *    \brief The TcpSocket class which implement tcp client
    *    \author Tyuryuchkin A.
    *    \version 0.0.1
    *    \date April 2019 года
    */
    class TcpClient: public AbstractSocket{
            socket_t _socket;        
            addr_t _addr;        
            addr_t _server_addr;        
            bool _is_connected;
            unsigned send(const char* src, size_t size, std::string addr, unsigned port){ return 0;}
        public:
            TcpClient();
            TcpClient(const TcpClient&);
            TcpClient(TcpClient&&) = delete;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            *    \param[in] addr Host address
            *    \param[in] port Port
            */
            bool open(const SockOpt = EmptyFlag, const std::string addr = "127.0.0.1", unsigned port = 0)override;
            /*!
            *    Connect to host 
            *    \param[in] addr Host address
            *    \param[in] port Port
            */
            bool connect(const std::string addr = "127.0.0.1", unsigned port = 0);
            /*!
            *    Read from socket
            *    \param[out] dest Pointer to destination buffer
            *    \param[in] size Function shall attempt read size bytes from socket
            *    \return Upon successful completion the function shall return number of bytes read
            */
            unsigned read(char* dest, size_t size)override;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            unsigned send(const char* src, size_t size)override;
            /*!
            *    Close socket
            *    \return Upon successful completion the function shall return true else false
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
            ~TcpClient();
    };
}
}
