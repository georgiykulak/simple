cmake_minimum_required( VERSION 3.19 )
project( multimethod )

set( CMAKE_CXX_STANDARD 11 ) # Required by task

add_executable( multimethod
    main.cpp Multimethod.h Shape.cpp Shape.h
)