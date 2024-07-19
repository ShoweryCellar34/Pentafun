set(POSITION_INDEPENDENT_CODE true)

cmake_minimum_required(VERSION 3.15)
project(Pentafun DESCRIPTION "A Basic Game Engine" LANGUAGES CXX HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentafun")
add_executable(Pentafun)

add_subdirectory(Pentagram)
add_dependencies(Pentafun Pentagram)
target_include_directories(Pentafun PUBLIC Pentagram)
target_link_libraries(Pentafun PUBLIC Pentagram)

target_sources(Pentafun PUBLIC "source/fun.cpp")

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/res COMMENT "Copying resources into binary directory")
