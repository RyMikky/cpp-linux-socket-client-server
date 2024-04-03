cmake_minimum_required(VERSION 3.11)
project(CPP_LINUX_SOCKET_TASK CXX)
set(CMAKE_CXX_STANDARD 20)

if (UNIX AND NOT APPLE) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
elseif(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
message(STATUS "Directory of sources: ${SRC_DIR}")

if (UNIX AND NOT APPLE) 

    add_library(Utils 

        ${SRC_DIR}/includes/base_includes.h
        ${SRC_DIR}/arguments/arguments.cpp
        ${SRC_DIR}/arguments/arguments.h

        ${SRC_DIR}/time/time.h

    )

    add_executable(server
        ${SRC_DIR}/main/main_server.cpp

        ${SRC_DIR}/server/server.cpp
        ${SRC_DIR}/server/server.h
    )

    target_include_directories(server PUBLIC Utils)
    target_link_libraries(server PUBLIC Utils) 
    target_link_libraries(server PRIVATE stdc++fs)

    add_executable(client
        ${SRC_DIR}/main/main_client.cpp

        ${SRC_DIR}/client/client.cpp
        ${SRC_DIR}/client/client.h
    )

    target_include_directories(client PUBLIC Utils)
    target_link_libraries(client PUBLIC Utils) 
    target_link_libraries(client PRIVATE stdc++fs)

endif()

