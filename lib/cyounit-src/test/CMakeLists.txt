add_executable(cyounit-test test-main.c
               ${CMAKE_SOURCE_DIR}/src/cyounit.c
               test-success.c)

set(CMAKE_C_FLAGS --coverage)

target_link_libraries(cyounit-test cyounit)
add_dependencies(cyounit-test cyounit)
