#pragma once

/*!
 * \file
 * \brief AbstractSocket header file
 *
 */

#if defined(__unix__) || defined(__APPLE__)
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <string>

namespace cppsocket{
    typedef unsigned SockOpt;
#if defined(__unix__) || defined(__APPLE__)
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
    static constexpr SockOpt NoDelayOpt = 0x10;///<to disable Nagle algorithm
    /*!
    *    \class AbstractSocket
    *    \brief Basic interface for socket
    *    \author Tyuryuchkin A.
    *    \version 0.3.1
    *    \date July 2019 года
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
            virtual ssize_t read(char* dest, size_t size) = 0;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            virtual ssize_t send(const char* src, size_t size) = 0;
            /*!
            *    Send to socket
            *    \param[in] src Pointer to source buffer (data)
            *    \param[in] size Function shall send size bytes to socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return Upon successful completion the function shall return number of bytes sent
            */
            virtual ssize_t send(const char* src, size_t size, std::string addr, unsigned port) = 0;
            /*!
            *    Open socket
            *    \param[in] sockOpt Options
            */
            virtual bool open(const SockOpt opt) = 0;
            /*!
            *    Close socket
            *    \return Upon successful completion the function shall return true else false
            */
            virtual bool close() = 0;
            /*!
            *    Flush buffered socket data
            */
            virtual void flush() = 0;   
        public:
            /*!
            *    Bind socket
            *    \param[in] addr Host address
            *    \param[in] port Port
            *    \return Upon successful completion the function shall return true else false
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
            /*!
            *    Set receive buffer size
            *    \param[in] size Size
            *    \return Upon successful completion the function shall return true else false
            */
            bool setSizeRecvBuffer(unsigned size){
                return setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&size), sizeof(size)) == 0? true: false;
            }
            /*!
            *    Set send buffer size
            *    \param[in] size Size
            *    \return Upon successful completion the function shall return true else false
            */
            bool setSizeSendBuffer(unsigned size){
                return setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&size), sizeof(size)) == 0? true: false;
            }
            /*!
            *    Set device to bind
            *    \param[in] dev Device name
            *    \return Upon successful completion the function shall return true else false
            */
            bool setDevice(const std::string& dev){
                const char* device = dev.c_str();
                unsigned size = static_cast<unsigned>(dev.size());
#if defined(__unix__)
                return setsockopt(_socket, SOL_SOCKET, SO_BINDTODEVICE, device, size) == 0? true: false;
#else
                return false;
#endif
            }
    };
};
