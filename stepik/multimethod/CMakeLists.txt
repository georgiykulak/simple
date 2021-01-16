cmake_minimum_required( VERSION 3.19 )

project( multimethod )

set( CMAKE_CXX_STANDARD 20 ) # But c++11 is required by task

add_executable( multimethod
    main.cpp Multimethod.h Shape.cpp Shape.h
)