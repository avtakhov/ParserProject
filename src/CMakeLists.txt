cmake_minimum_required(VERSION 3.0)

enable_testing()

project(ParserProject)
set(BUILD_GMOCK OFF CACHE BOOL "Build only googletest")
set(gtest_force_shared_crt ON CACHE BOOL "Use Multi-threaded DLL Runtime Library")
add_subdirectory(deps/gtest)

add_subdirectory(params_parser)
add_subdirectory(parser_tests)

add_dependencies(ParserTests ParamsParser gtest gtest_main)

