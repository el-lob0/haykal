cmake_minimum_required(VERSION 3.10)
project(haykal C)

set(CMAKE_C_STANDARD 99)

# --- Find GLFW via pkg-config ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# --- Build the shared library ---
add_library(haykal SHARED src/haykal.c)

# --- Include dirs ---
target_include_directories(haykal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
)

# --- Link GLFW, dl, m ---
target_link_libraries(haykal
    ${GLFW_LIBRARIES}   # pkg-config resolved libs
    dl
    m
    freetype
)
