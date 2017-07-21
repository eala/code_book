cmake_minimum_required (VERSION 3.1) 

project(Advanced_UNIX_practice)

find_package(Threads REQUIRED)

set(INCLUDES
    apue.h
    error.c
)

set(FETCH_SRCS
    fetchExitCode_ch11_p389.c
)

set(TARGET fetchExitCode)
add_executable(${TARGET} ${FETCH_SRCS} ${INCLUDES})
target_link_libraries(${TARGET} Threads::Threads)
