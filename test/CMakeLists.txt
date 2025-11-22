cmake_minimum_required(VERSION 3.10)
project(test C)

# =======================================================
# 1. Locate the custom shared library (libhaykal.so)
# =======================================================
# Search for libhaykal.so in the 'include' directory



add_executable(test src/main.c)

# --- Find GLFW via pkg-config ---
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(HAYKAL REQUIRED haykal)

find_package(Freetype REQUIRED)

# --- Include dirs ---
# You already have the directory with libhaykal.so included, which is good for headers.
target_include_directories(test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
    ${HAYKAL_INCLUDE_DIRS}
    ${NIB_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

# --- Link Libraries ---
target_link_libraries(test
    ${HAYKAL_LIBRARY}      # <-- LINK TO YOUR FOUND LIBRARY VARIABLE
    ${GLFW_LIBRARIES}
    ${NIB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    dl
    m
    freetype
)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g -O1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
