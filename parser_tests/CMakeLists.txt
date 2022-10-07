cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
set(SRC_PATH "${PROJECT_SOURCE_DIR}")
set(GTEST_PATH "${PROJECT_SOURCE_DIR}/deps/gtest/googletest/include")
list(APPEND EXTRA_LIBS ParamsParser gtest gtest_main)
list(APPEND EXTRA_INCLUDES ${SRC_PATH} ${GTEST_PATH})

add_executable(ParserTests basic_suite.cpp errors_suite.cpp extra_suite.cpp)
target_link_libraries(ParserTests ${EXTRA_LIBS})
target_include_directories(ParserTests PUBLIC ${EXTRA_INCLUDES})

add_test(NAME ParserTests COMMAND ParserTests)

