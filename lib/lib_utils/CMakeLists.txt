cmake_minimum_required(VERSION 3.16)
project(lib_utils)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wconversion -Wpedantic)

include_directories(include/lib_utils)

set (HEADER
        include/lib_utils/Command.h
        include/lib_utils/Console.h
        include/lib_utils/FileIO.h
        include/lib_utils/Misc.h
        )
set (SOURCE
        src/Command.cpp
        src/Console.cpp
        src/FileIO.cpp
        src/Misc.cpp
        )

#only for testing
#add_executable(${PROJECT_NAME} ${HEADER} ${SOURCE})

add_library(${PROJECT_NAME} ${HEADER} ${SOURCE})
target_compile_options(${PROJECT_NAME} PUBLIC ${UTILS_CXX_FLAGS})
