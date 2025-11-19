cmake_minimum_required(VERSION 3.10)
project(haykal C)

add_library(haykal SHARED src/haykal.c)

target_include_directories(haykal PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
