cmake_minimum_required( VERSION 3.19 )

project( socket_server )

set( CMAKE_CXX_STANDARD 20 )

add_executable( server server.cpp )

add_executable( client client.cpp )