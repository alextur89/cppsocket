#pragma once

/*!
 * \file
 * \brief AbstractSocket header file
 *
 */

#include <arpa/inet.h>
#include <string>

namespace cppsocket{
    typedef unsigned SockOpt;
    typedef int socket_t;///socket's descriptor
    typedef struct sockaddr_in addr_t;///address
    ///Socket flags for open method
    enum SockFlags{
        EmptyFlag = 0,///<empty
        Reuseaddr = 0x1,///<equivalent to the SO_REUSEADDR socket option
        Nonblock = 0x2,///<not blocking io
        Broadcast = 0x4,///<broadcast io
        Bind = 0x8,///<to bind interface (must be for recv socket)
        NoDelay = 0x10///<to disable Nagle algorithm
    };
    /*!
    *    \class AbstractSocket
    *    \brief Basic interface for socket
    *    \author Tyuryuchkin A.
    *    \version 0.0.1
    *    \date Febrary 2019 года
    */
    class AbstractSocket{
        protected:
            /*!
            *    Read from socket
            *    \param[out] dest Pointer to destination buffer
            *    \param[in] size Function shall attempt read size bytes from socket
            *    \return Upon successful completion the function shall return number of bytes read
            */
            virtual unsigned read(char* dest, size_t size) = 0;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            virtual unsigned send(const char* src, size_t size) = 0;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            virtual unsigned send(const char* src, size_t size, std::string addr, unsigned port) = 0;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            *    \param[in] addr Host address
            *    \param[in] port Port
            */
            virtual bool open(const SockOpt opt, const std::string addr, unsigned port) = 0;
            /*!
            *    Close socket
            *    \return Upon successful completion the function shall return true else false
            */
            virtual bool close() = 0;
            /*!
            *    Flush buffered socket data
            */
            virtual void flush() = 0;   
    };
};
