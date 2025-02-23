cmake_minimum_required(VERSION 3.10)
project(bind VERSION 0.1.0 LANGUAGES C)

add_library(bind STATIC 
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

target_include_directories(bind PUBLIC "include/")
# target_compile_definitions(bind PRIVATE BIND_EXPORTS)

# For Windows, export the symbols
if(WIN32)
    set_target_properties(bind PROPERTIES PREFIX "" SUFFIX ".lib")
endif()

include(CTest)
enable_testing()

add_executable(my_tests testing/test_main.c)
target_link_libraries(my_tests PRIVATE bind)

add_test(NAME MyTest COMMAND $<TARGET_FILE:my_tests>)