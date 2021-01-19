cmake_minimum_required(VERSION 3.17)
project(Block5 C)

set(CMAKE_C_STANDARD 99)

add_executable(ntpclient main.c src/NTP_client.c header/NTP_client.h)