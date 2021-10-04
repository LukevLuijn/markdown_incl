cmake_minimum_required(VERSION 3.16)
project(markdown_incl)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include/${PROJECT_NAME})
include_directories(src/)

set(HEADERS include/${PROJECT_NAME}/Setup.h include/${PROJECT_NAME}/Convert.h)
set(SOURCES src/main.cpp src/Setup.cpp src/Convert.cpp)

add_subdirectory(../lib/lib_utils lib_utils)
include_directories(../lib/lib_utils/include/lib_utils)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND /bin/bash ~/scripts/log.sh -i -m 'build complete')

target_compile_options(${PROJECT_NAME} PUBLIC ${UTILS_CXX_FLAGS})
target_link_libraries(${PROJECT_NAME} lib_utils ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR})

