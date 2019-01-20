#include <gtest/gtest.h>
#include "udpsocket.h"

class TestUdpSocket : public ::testing::Test
{
protected:
	void SetUp()
	{
        socket = new cppsocket::udp::UdpSocket();
	}
	void TearDown()
	{
		delete socket;
	}
    cppsocket::udp::UdpSocket *socket;
};

TEST_F(TestUdpSocket, OpenListenSocket) {
    ASSERT_EQ(socket->open("127.0.0.1", 12345, cppsocket::Reuseaddr | cppsocket::Bind), true);
}

TEST_F(TestUdpSocket, OpenSendSocket) {
    ASSERT_EQ(socket->open("127.0.0.1", 12345, cppsocket::Reuseaddr), true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
