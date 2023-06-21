cmake_minimum_required(VERSION 3.25)
project(AOCS_Connector C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(AOCS_Connector
        main.c tcp_connect.c tcp_connect.h)

target_link_libraries(AOCS_Connector ${GTK3_LIBRARIES} m)
