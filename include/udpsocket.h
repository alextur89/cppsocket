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
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class UdpSocket: public AbstractSocket{
        public:
            UdpSocket();
            UdpSocket(const UdpSocket&);
            UdpSocket(UdpSocket&&) = delete;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            */
            bool open(const SockOpt = EmptyFlagOpt)override;
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
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            ssize_t send(const char* src, size_t size, std::string addr, unsigned port)override;
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
    *     \class UdpSelect
    *     \brief The UdpSelect class which implement synchronous input multiplexing
    *     \author Tyuryuchkin A.
    *     \version 0.3.0
    *     \date July 2019 года
    */
    class UdpSelect{
        private:
            std::list<std::shared_ptr<UdpSocket> > _sockets;
        public:
            ///Options for waiting data
            enum class Wait: long{
                infinity = -1,///<infinity
                immediately = 0///<immediately
            };
            UdpSelect() = default;
            /*!
            *    Append socket to set for sync. 
            *    \param[in] sock Socket
            */
            void append(const UdpSocket& sock);
            /*!
            *    Wait for input data
            *    \param[out] result Pointers list of ready for input sockets
            *    \param[in] microsec Pause for waiting sync.
            */
            void ready(std::list<std::shared_ptr<UdpSocket> >& result, long microsec = static_cast<long>(Wait::infinity));
    };
}
}
