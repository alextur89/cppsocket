#pragma once

/*!
 * \file
 * \brief UdpSocket header file
 *
 */

#include "abstractsocket.h"
#include <list>
#include <memory>

namespace cppsocket{
namespace udp{
    /*!
    *    \class UdpSocket
    *    \brief The UdpSocket class which implement interface of AbstractSocket
    *    \author Tyuryuchkin A.
    *    \version 0.0.1
    *    \date Febrary 2019 года
    */
    class UdpSocket: public AbstractSocket{
            socket_t _socket;        
            addr_t _addr;        
        public:
            UdpSocket();
            UdpSocket(const UdpSocket&);
            UdpSocket(UdpSocket&&) = delete;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            *    \param[in] addr Host address
            *    \param[in] port Port
            */
            bool open(const SockOpt = EmptyFlag, const std::string addr = "127.0.0.1", unsigned port = 0)override;
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
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            unsigned send(const char* src, size_t size, std::string addr, unsigned port)override;
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
            ~UdpSocket();
    };
    /*!
    *     \class UdpClient
    *     \brief The UdpClient class which implement synchronous input multiplexing
    *     \author Tyuryuchkin A.
    *     \version 0.0.1
    *     \date Febrary 2019 года
    */
    class UdpClient{
        private:
            std::list<UdpSocket> _sockets;
        public:
            ///Options for waiting data
            enum class Wait: long{
                infinity = -1,///<infinity
                immediately = 0///<immediately
            };
            UdpClient() = default;
            /*!
            *    Append socket to set for sync. 
            *    \param[in] sock Socket
            */
            void append(const UdpSocket& sock);
            /*!
            *    Append socket to set for sync. 
            *    \param[in] list List of sockets
            */
            void append(std::initializer_list<UdpSocket> list);
            /*!
            *    Wait for input data
            *    \param[out] result List of ready for input sockets
            *    \param[in] microsec Pause for waiting sync.
            */
            void ready(std::list<UdpSocket>& result, long microsec = static_cast<long>(Wait::infinity));
    };
}
}
