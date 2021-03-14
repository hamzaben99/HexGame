add_library(cyounit STATIC
            cyounit.c
            cyounit_private.h)

target_include_directories(cyounit PUBLIC ${CMAKE_SOURCE_DIR}/include/)

install(TARGETS cyounit DESTINATION lib)
install(FILES ../include/cyounit.h DESTINATION include)
