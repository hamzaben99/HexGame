use_c99()

set(SET_PATH ${CMAKE_SOURCE_DIR}/lib/set-src)
set(GRAPH_PATH ${CMAKE_SOURCE_DIR}/src)

add_executable(server server.c
               player_loader.c
               $<TARGET_OBJECTS:graph>
               $<TARGET_OBJECTS:set>)
target_link_libraries(server m gsl gslcblas)
if (CMAKE_DL_LIBS)
	target_link_libraries(server ${CMAKE_DL_LIBS})
endif ()

# Do not add "lib" prefix on generated .so files
set(CMAKE_SHARED_LIBRARY_PREFIX_C "")

# Generate random player strategy
add_library(random_player SHARED
            random_player.c
            $<TARGET_OBJECTS:graph>
            $<TARGET_OBJECTS:set>)
target_link_libraries(random_player gsl gslcblas)

add_library(connex_player0 SHARED
            connex_player.c
            WTree.c
            methods.c
            $<TARGET_OBJECTS:graph>
            $<TARGET_OBJECTS:set>)
target_link_libraries(connex_player0 gsl gslcblas)

add_library(connex_player1 SHARED
            connex_player.c
            WTree.c
            methods.c
            $<TARGET_OBJECTS:graph>
            $<TARGET_OBJECTS:set>)
target_link_libraries(connex_player1 gsl gslcblas)

add_library(connex_player2 SHARED
            connex_player.c
            WTree.c
            methods.c
            $<TARGET_OBJECTS:graph>
            $<TARGET_OBJECTS:set>)
target_link_libraries(connex_player2 gsl gslcblas)

#add_library(minimax_player SHARED
#            minimax_player.c
#            WTree.c
#            methods.c
#            $<TARGET_OBJECTS:graph>
#            $<TARGET_OBJECTS:set>)
#target_link_libraries(minimax_player gsl gslcblas)   

# Generate set library
add_library(set OBJECT
            ${SET_PATH}/src/set_dynamic.c)
target_include_directories(set PUBLIC ${SET_PATH}/include)
target_compile_options(set PUBLIC -fPIC)

# Generate graph library
add_library(graph OBJECT
            ${GRAPH_PATH}/graph.c)
target_compile_options(graph PUBLIC -fPIC)
target_compile_options(connex_player0 PUBLIC -Wall -D METHOD=0)
target_compile_options(connex_player1 PUBLIC -Wall -D METHOD=1)
target_compile_options(connex_player2 PUBLIC -Wall -D METHOD=2)
