#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "../src/player_loader.h"
#include "test_player_loader.h"
#include "cyounit.h"
#include <unistd.h>

cy_new_test(check_if_return_correct_player_with_valid_path) {
	char cwd[400];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/install/random_player.so");
	
	void *handler = dlopen(cwd, RTLD_LAZY);

	struct player p1;
	p1.handler = handler;
    p1.get_player_name = dlsym(handler, "get_player_name");
    p1.propose_opening = dlsym(handler, "propose_opening");
    p1.accept_opening = dlsym(handler, "accept_opening");
    p1.initialize_graph = dlsym(handler, "initialize_graph");
    p1.initialize_color = dlsym(handler, "initialize_color");
    p1.play = dlsym(handler, "play");
    p1.finalize = dlsym(handler, "finalize");
	
	struct player p2 = initialize_player(cwd);

	cy_assert(p1.handler == p2.handler, "Two player have the same handler...");
	cy_assert(p1.get_player_name == p2.get_player_name, "Two player have the same get_player_name function...");
	cy_assert(p1.propose_opening == p2.propose_opening, "Two player have the same propose_opening function...");
	cy_assert(p1.accept_opening == p2.accept_opening, "Two player have the same accept_opening function...");
	cy_assert(p1.initialize_graph == p2.initialize_graph, "Two player have the same initialize_graph function...");
	cy_assert(p1.initialize_color == p2.initialize_color, "Two player have the same initialize_color function...");
	cy_assert(p1.play == p2.play, "Two player have the same play function...");
	cy_assert(p1.finalize == p2.finalize, "Two player have the same finalize function...");

	close_player(p1);
	close_player(p2);
	
	cy_display_result();
	cy_return_result();
}

cy_new_test(test_initialize_player) {
	cy_announce_test("Testing function initialize_player...");
	cy_run_test(check_if_return_correct_player_with_valid_path);
	cy_display_result();
	cy_return_result();
}

cy_new_test(check_return_zero_when_valid_player) {
	char cwd[400];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/install/random_player.so");
	struct player p = initialize_player(cwd);
	cy_assert(close_player(p) == 0, "Player with valid handler return zero...");
	cy_return_result();
}

cy_new_test(test_close_player) {
	cy_announce_test("Testing function close_player...");
	cy_run_test(check_return_zero_when_valid_player);
	cy_display_result();
	cy_return_result();
}

cy_new_test(test_player_loader) {
	cy_announce_test("Testing file player_loader");
	cy_run_test(test_initialize_player);
	cy_run_test(test_close_player)
	cy_display_result();
	cy_return_result();
}
