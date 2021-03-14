use_c99()

set(CYOUNIT_PATH ${CMAKE_SOURCE_DIR}/lib/cyounit)
link_directories(${CYOUNIT_PATH}/lib)

# Add compilation with gcov
set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(alltests_project main.c
			   test_player_loader.c
			   ${CMAKE_SOURCE_DIR}/src/player_loader.c
			   test_graph_generator.c
			   test_set.c
			   $<TARGET_OBJECTS:graph>
			   $<TARGET_OBJECTS:set>)

add_executable(alltests_random_player main.c 
			   test_random_player.c 
			   ${CMAKE_SOURCE_DIR}/src/random_player.c 
			   $<TARGET_OBJECTS:graph>
			   $<TARGET_OBJECTS:set>)

add_executable(alltests_connex_player main.c 
			   test_connex_player.c
			   test_WTree.c
			   test_methods.c
			   ${CMAKE_SOURCE_DIR}/src/connex_player.c
			   ${CMAKE_SOURCE_DIR}/src/WTree.c
        	   ${CMAKE_SOURCE_DIR}/src/methods.c
			   $<TARGET_OBJECTS:graph>
			   $<TARGET_OBJECTS:set>)

function(link TARGET)
		   target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src
                           ${CMAKE_SOURCE_DIR}/lib/set-src/include)

			if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
			   	add_compile_options(${TARGET} -lgcov)
			endif()

			if (CMAKE_DL_LIBS)
				target_link_libraries(${TARGET} ${CMAKE_DL_LIBS})
			endif ()

			# Link libs
			target_link_libraries(${TARGET} m gsl gslcblas)

			# Link to Cyounit
			target_include_directories(${TARGET} PRIVATE ${CYOUNIT_PATH}/include)
			target_link_libraries(${TARGET} cyounit)
endfunction(link)

# Add specific debug options
target_compile_options(alltests_project PUBLIC -g -Wall -DTEST -DPROJECT)
target_compile_options(alltests_random_player PUBLIC -g -Wall -DTEST -DRANDOM)
target_compile_options(alltests_connex_player PUBLIC -g -Wall -DTEST -DCONNEX)

link(alltests_project)
link(alltests_random_player)
link(alltests_connex_player)