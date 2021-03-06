cmake_minimum_required(VERSION 2.8)
#project(cppsocket)
ADD_LIBRARY(cppsocket STATIC source/udpsocket.cpp source/tcpsocket.cpp)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

#find_package(GTest REQUIRED)

add_definitions(-std=c++11)
include_directories(include source testing)
set(UDPTESTSOURCE source/udpsocket.cpp tests/testudp.cpp)

add_executable(TestUdpSocket ${UDPTESTSOURCE})

target_link_libraries(TestUdpSocket gtest_main cppsocket)

enable_testing()
add_test(TestUdpSocket "./TestUdpSocket")

add_custom_target(
  doc ALL
  doxygen COMMAND
)


