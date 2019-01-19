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
}

TEST_F(TestUdpSocket, OpenSendSocket) {
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
