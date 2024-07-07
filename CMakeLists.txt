cmake_minimum_required(VERSION 3.15)
project(Pentafun DESCRIPTION "A Basic Game Engine" LANGUAGES CXX HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentafun")
add_executable(Pentafun)

add_subdirectory(Pentagram)
target_include_directories(Pentafun PUBLIC Pentagram)
target_link_libraries(Pentafun PUBLIC Pentagram)

target_sources(Pentafun PUBLIC "fun.cpp")
file(REMOVE ${CMAKE_BINARY_DIR}\\res)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
