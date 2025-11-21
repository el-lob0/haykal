set(PKG_CONFIG_LIBDIR "/usr/local/lib/pkgconfig")

cmake_minimum_required(VERSION 3.10)
project(haykal C)

set(CMAKE_C_STANDARD 99)

# --- Find GLFW via pkg-config ---
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(NIB REQUIRED nib)

find_package(Freetype REQUIRED)

# --- Build the shared library ---
add_library(haykal SHARED src/haykal.c)

# --- Include dirs ---
target_include_directories(haykal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
    ${NIB_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

# --- Link GLFW, dl, m ---
target_link_libraries(haykal
    ${GLFW_LIBRARIES}   # pkg-config resolved libs
    ${NIB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    dl
    m
    freetype
)
