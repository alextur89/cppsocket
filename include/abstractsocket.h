#pragma once

/*!
 * \file
 * \brief AbstractSocket header file
 *
 */

#if defined(linux) || defined(__APPLE__)
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <arpa/inet.h>
#include <string>

namespace cppsocket{
    typedef unsigned SockOpt;
#if defined(linux) || defined(__APPLE__)
    typedef int socket_t;///socket's descriptor
    typedef struct sockaddr_in addr_t;///address
    typedef struct sockaddr saddr_t;///address
#else
    typedef SOCKET socket_t;///socket's descriptor
    typedef SOCKADDR_IN addr_t;///address
    typedef SOCKADDR saddr_t;///address
#endif
    ///Socket flags for open method
    using SockOpt = unsigned int;
    static constexpr SockOpt EmptyFlagOpt = 0;///<empty
    static constexpr SockOpt ReuseaddrOpt = 0x1;///<equivalent to the SO_REUSEADDR socket option
    static constexpr SockOpt NonblockOpt = 0x2;///<not blocking io
    static constexpr SockOpt BroadcastOpt = 0x4;///<broadcast io
    static constexpr SockOpt BindOpt = 0x8;///<to bind interface (must be for recv socket)
    static constexpr SockOpt NoDelayOpt = 0x10;///<to disable Nagle algorithm
    /*!
    *    \class AbstractSocket
    *    \brief Basic interface for socket
    *    \author Tyuryuchkin A.
    *    \version 0.0.1
    *    \date Febrary 2019 года
    */
    class AbstractSocket{
        protected:
            socket_t _socket;
            addr_t _addr;        
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
            /*!
            *    Bind socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            */
            bool bind(const std::string addr, unsigned port){
                if (addr.empty()){
                    return false;
                }
                _addr.sin_family = AF_INET;
                _addr.sin_addr.s_addr = inet_addr(addr.c_str());
                _addr.sin_port = htons(port);
                if (::bind(_socket, (struct sockaddr*) &_addr, sizeof(struct sockaddr_in)) == -1){
                    return false;
                }
                return true;
            }
    };
};
