#include <gtest/gtest.h>
#include "udpsocket.h"
#include <algorithm>
#include <memory>

class TestUdpSocket : public ::testing::Test
{
protected:
	void SetUp()
	{
	}
	void TearDown()
	{
	}
    cppsocket::udp::UdpSocket send_socket;
    cppsocket::udp::UdpSocket listen_socket;
    cppsocket::udp::UdpSelect selector;
};

TEST_F(TestUdpSocket, OpenListenSocket) {
    ASSERT_EQ(listen_socket.open(cppsocket::ReuseaddrOpt | cppsocket::BindOpt, "127.0.0.1", 12345), true);
}

TEST_F(TestUdpSocket, OpenSendSocket) {
    ASSERT_EQ(send_socket.open(cppsocket::ReuseaddrOpt, "127.0.0.1", 12345), true);
}

TEST_F(TestUdpSocket, PureExchange) {
    listen_socket.open(cppsocket::ReuseaddrOpt | cppsocket::BindOpt, "127.0.0.1", 12345);
    send_socket.open(cppsocket::ReuseaddrOpt);
    
    char data[] = {1,2,3,4,5,6,7,8};
    ASSERT_EQ(send_socket.send(data, 8, "127.0.0.1", 12345) > 0, true);
    char buf[] = {0,0,0,0,0,0,0,0};
    ASSERT_EQ(listen_socket.read(buf, 8) > 0, true);
    ASSERT_EQ(std::equal(std::begin(data), std::end(data), std::begin(buf)), true);
}

TEST_F(TestUdpSocket, SelectExchange) {
    listen_socket.open(cppsocket::ReuseaddrOpt | cppsocket::BindOpt, "127.0.0.1", 12345);
    send_socket.open(cppsocket::ReuseaddrOpt);
    selector.append(listen_socket);

    char data[] = {1,2,3,4,5,6,7,8};
    ASSERT_EQ(send_socket.send(data, 8, "127.0.0.1", 12345) > 0, true);
    char buf[] = {0,0,0,0,0,0,0,0};

    std::list<std::shared_ptr<cppsocket::udp::UdpSocket> > vs;
    selector.ready(vs, 0);
    ASSERT_EQ(vs.empty(), false);
    std::shared_ptr<cppsocket::udp::UdpSocket> sock = vs.front();
    ASSERT_EQ(sock->read(buf, 8) > 0, true);
    ASSERT_EQ(std::equal(std::begin(data), std::end(data), std::begin(buf)), true);
}

TEST_F(TestUdpSocket, SelectFailExchange) {
    listen_socket.open(cppsocket::ReuseaddrOpt | cppsocket::BindOpt, "127.0.0.1", 12345);
    send_socket.open(cppsocket::ReuseaddrOpt);
    selector.append(listen_socket);

    char data[] = {1,2,3,4,5,6,7,8};
    ASSERT_EQ(send_socket.send(data, 8, "127.0.0.1", 11111) > 0, true);
    char buf[] = {0,0,0,0,0,0,0,0};

    std::list<std::shared_ptr<cppsocket::udp::UdpSocket> > vs;
    selector.ready(vs, 10);
    ASSERT_EQ(vs.empty(), true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
