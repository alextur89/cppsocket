#pragma once

/*!
 * \file
 * \brief Exceptions for UdpSocket header file
 *
 */

#include <exception>
#include <string>

namespace cppsocket{
    /*!
    *    \class UdpSocketException
    *    \brief Basic exception class
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class SocketException: public std::exception{
        protected:
            std::string msg;///<Exception's message
        public:
            const char* what() const noexcept{
                return msg.c_str();
            };
    };
    /*!
    *    \class ExcOpenSocket
    *    \brief Exception throw when socket not open
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class ExcOpenSocket: public SocketException{
        public:
            ExcOpenSocket(){
                msg = "Socket not open";
            }
    };
    /*!
    *    \class ExcBind
    *    \brief Exception throw when socket cant bind to interface
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class ExcBind: public SocketException{
        public:
            ExcBind(){
                msg = "Bind has failed";
            }
    };
    /*!
    *    \class ExcEmptySocketContainer
    *    \brief Exception throw when client wait sync from empty socket set
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class ExcEmptySocketContainer: public SocketException{
        public:
            ExcEmptySocketContainer(){
                msg = "Sockets container is empty";
            }
    };
    /*!
    *    \class ExcBadDescriptor
    *    \brief Exception throw when socket's descriptor < 0
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class ExcBadDescriptor: public SocketException{
        public:
            ExcBadDescriptor(){
                msg = "Bad sockets descriptor";
            }
    };
    /*!
    *    \class ExcNotConnected
    *    \brief Exception throw when the programm attempt to transfer data, but the socket not connected 
    *    \author Tyuryuchkin A.
    *    \version 0.3.0
    *    \date July 2019 года
    */
    class ExcNotConnected: public SocketException{
        public:
            ExcNotConnected(){
                msg = "Not connected";
            }
    };
}
