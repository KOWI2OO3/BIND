cmake_minimum_required(VERSION 3.5.0)
project(bind VERSION 0.1.0 LANGUAGES C)

add_library(bind 
    src/bind.c
    src/bytebuffer.c
    src/bytebuffer.h
)

target_include_directories(bind PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(CTest)
enable_testing()

add_executable(my_tests testing/test_main.c)
target_link_libraries(my_tests PRIVATE bind)

add_test(NAME MyTest COMMAND $<TARGET_FILE:my_tests>)