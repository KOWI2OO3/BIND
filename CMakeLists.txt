cmake_minimum_required(VERSION 3.10)
project(bind VERSION 0.1.0 LANGUAGES C)

add_library(bind 
    src/bytebuffer.c
    src/bufferhelper.c
    src/bindtypes.c
    src/bindwriter.c
    src/bindreader.c
    src/hashmap/hashmap.c
)

if (UNIX)
    target_link_libraries(bind PRIVATE m)
endif (UNIX)

target_include_directories(bind PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(CTest)
enable_testing()

add_executable(my_tests testing/test_main.c)
target_link_libraries(my_tests PRIVATE bind)

add_test(NAME MyTest COMMAND $<TARGET_FILE:my_tests>)