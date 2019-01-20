#pragma once

#include <exception>
#include <string>

namespace cppsocket{
    class UdpSocketException: public std::exception{
        protected:
            std::string msg;
        public:
            const char* what() const noexcept{
                return msg.c_str();
            };
    };
    class ExcSetSockOpt: public UdpSocketException{
        public:
            ExcSetSockOpt(){
                msg = "Option not set";
            }
    };
    class ExcBind: public UdpSocketException{
        public:
            ExcBind(){
                msg = "Bind has failed";
            }
    };
    class ExcEmptySocketContainer: public UdpSocketException{
        public:
            ExcEmptySocketContainer(){
                msg = "Sockets container is empty";
            }
    };
    class ExcOpenSocket: public UdpSocketException{
        public:
            ExcOpenSocket(){
                msg = "Socket not open";
            }
    };
}
