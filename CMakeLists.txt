cmake_minimum_required(VERSION 2.8)
project(cppsocket)

find_package(GTest REQUIRED)

include_directories(include source testing)
set(UDPTESTSOURCE source/udpsocket.cpp tests/testudp.cpp)

add_executable(TestUdpSocket ${UDPTESTSOURCE})

target_link_libraries(TestUdpSocket ${GTEST_LIBRARIES})

enable_testing()
add_test(TestUdpSocket "./TestUdpSocket")
