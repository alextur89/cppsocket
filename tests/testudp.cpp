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
        send_socket.close();
        listen_socket.close();
	}
    cppsocket::udp::UdpSocket send_socket;
    cppsocket::udp::UdpSocket listen_socket;
    cppsocket::udp::UdpClient client;
};

TEST_F(TestUdpSocket, OpenListenSocket) {
    ASSERT_EQ(listen_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr | cppsocket::Bind), true);
}

TEST_F(TestUdpSocket, OpenSendSocket) {
    ASSERT_EQ(send_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr), true);
}

TEST_F(TestUdpSocket, PureExchange) {
    listen_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr | cppsocket::Bind);
    send_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr);
    
    char data[] = {1,2,3,4,5,6,7,8};
    ASSERT_EQ(send_socket.send(data, 8) > 0, true);
    char buf[] = {0,0,0,0,0,0,0,0};
    ASSERT_EQ(listen_socket.read(buf, 8) > 0, true);
    ASSERT_EQ(std::equal(std::begin(data), std::end(data), std::begin(buf)), true);
}

TEST_F(TestUdpSocket, ClientExchange) {
    listen_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr | cppsocket::Bind);
    send_socket.open("127.0.0.1", 12345, cppsocket::Reuseaddr);
    client.append(listen_socket);

    char data[] = {1,2,3,4,5,6,7,8};
    ASSERT_EQ(send_socket.send(data, 8) > 0, true);
    char buf[] = {0,0,0,0,0,0,0,0};

    std::list<cppsocket::udp::UdpSocket> vs;
    client.ready(vs, 0);
    ASSERT_EQ(vs.empty(), false);
    cppsocket::udp::UdpSocket sock = vs.front();
    ASSERT_EQ(sock.read(buf, 8) > 0, true);
    ASSERT_EQ(std::equal(std::begin(data), std::end(data), std::begin(buf)), true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
