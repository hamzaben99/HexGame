#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_spmatrix.h>
#include <time.h>
#include <unistd.h>

#include "../src/player.h"
#include "../src/player_loader.h"
#include "test_random_player.h"
#include "cyounit.h"
#include "graph_manager.h"
#include "../lib/set-src/include/set.h"

struct random_player {
    struct graph_t *player_graph;
    enum color_t player_color;
    struct set *available_vertices;
};

extern struct random_player p;

cy_new_test(player_name_returns_random_player) {
	cy_assert(!strcmp(get_player_name(), "Random Player"), "get_player_name returns the correct name...");
	cy_return_result();
}

cy_new_test(test_get_player_name) {
	cy_announce_test("Testing get_player_name")
	cy_run_test(player_name_returns_random_player);
	cy_display_result();
	cy_return_result();
}

cy_new_test(graph_variable_null_when_unitialized) {
	cy_assert(p.player_graph == NULL, "global variable is NULL when uninitialized...");
	cy_return_result();
}

cy_new_test(graph_updated_when_initialized) {
	struct graph_t *g = create_graph('c',5);
	initialize_graph(g);
	cy_assert(p.player_graph->num_vertices == g->num_vertices,
	          "num_vertices of global variable is updated when initialized...");

	finalize();
	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(test_initialize_graph) {
	cy_announce_test("Testing function initialize_graph...");

	cy_run_test(graph_variable_null_when_unitialized);
	cy_run_test(graph_updated_when_initialized);

	cy_display_result();
	cy_return_result();
}

cy_new_test(player_default_color_is_black) {
	cy_assert(p.player_color == BLACK, "player_color default value is BLACK...");
	cy_return_result();
}

cy_new_test(player_color_stays_black_after_setting_black) {
	initialize_color(BLACK);
	cy_assert(p.player_color == BLACK, "player_color still the same after the current player color initialization...");
	cy_return_result();
}

cy_new_test(player_color_is_updated) {
	initialize_color(WHITE);
	cy_assert(p.player_color == WHITE, "player_color is correctly updated...");
	cy_return_result();
}

cy_new_test(test_initialize_color) {
	cy_announce_test("Testing function initialize_color...");

	cy_run_test(player_default_color_is_black);
	cy_run_test(player_color_stays_black_after_setting_black);
	cy_run_test(player_color_is_updated)

	cy_display_result();
	cy_return_result();
}


cy_new_test(first_proposition_is_valid) {
	struct graph_t *g = create_graph('c', 6);
	initialize_graph(g);
	struct move_t move = propose_opening();
	cy_assert(gsl_spmatrix_get(p.player_graph->o, p.player_color, move.m) != 1, "first proposition is valid...");
	finalize(g);
	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(move_color_is_player_color) {
	struct graph_t *g = create_graph('c', 7);
	initialize_graph(g);
	struct move_t move = propose_opening();
	cy_assert(move.c == BLACK, "the move's color is BLACK");
	finalize(g);
	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(test_propose_opening) {
	cy_announce_test("Testing function propose_opening...");

	cy_run_test(first_proposition_is_valid);
	cy_run_test(move_color_is_player_color);

	cy_display_result();
	cy_return_result();
}

cy_new_test(random_number_between_zero_and_one) {
	struct graph_t *g = create_graph('c', 10);
	initialize_graph(g);
	struct move_t move = propose_opening();
	int bol = accept_opening(move); 
	cy_assert(bol == 0 || bol == 1, "Number is between 0 and 1...")
	finalize();
	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(test_accept_opening)
{
	cy_announce_test("Testing function accept_opening...");

	cy_run_test(random_number_between_zero_and_one);

	cy_display_result();
	cy_return_result();	
}

cy_new_test(graph_updated) {
	struct graph_t *g = create_graph('c', 5);
	initialize_graph(g);
	struct move_t prev_move;
	prev_move.m = 5;
    prev_move.c = WHITE;
	struct move_t move = play(prev_move);
	cy_assert(gsl_spmatrix_get(p.player_graph->o, p.player_color, move.m) == 1, "Graph is updated after playing");
	finalize(g);
	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(move_chosen_different_than_prev_move) {
	struct graph_t *g = create_graph('c', 5);
	initialize_graph(g);
	struct move_t not_move;
	not_move.m = 13;
    not_move.c = BLACK;
	struct move_t move1 = play(not_move);
	struct move_t move2 = play(move1);
	struct move_t move3 = play(move2);
	struct move_t move = play(move3);
	cy_assert(move.m != move1.m && move.m != move2.m && move.m != move3.m, "the move chosen is different than the old moves");
	finalize(g);
	cy_return_result();
}

cy_new_test(move_chosen_in_an_unoccupied_place) {
	struct graph_t *g = create_graph('h', 5);
	gsl_spmatrix_set(g->o, BLACK, 6, 1);
	gsl_spmatrix_set(g->o, BLACK, 8, 1);
	gsl_spmatrix_set(g->o, BLACK, 11, 1);
	gsl_spmatrix_set(g->o, BLACK, 12, 1);
	initialize_color(BLACK);
	initialize_graph(g);
	struct move_t not_move;
	not_move.m = 13;
    not_move.c = BLACK;
	struct set *possible_vertices = set__empty();
	int size = set__size(p.available_vertices);
	for (int i = 0; i < size; i++) {
		set__add(possible_vertices, set__value(p.available_vertices, i));
	}
	struct move_t move = play(not_move);
	cy_assert(move.m != 6 && move.m != 8 && move.m != 11 && move.m != 12 && set__find(possible_vertices, move.m), 
			  "The move chosen is in an unoccupied vertex");
	finalize(g);
	set__free(possible_vertices);
	cy_return_result();
}

cy_new_test(test_play) {
	cy_announce_test("Testing play");

	cy_run_test(graph_updated);
	cy_run_test(move_chosen_different_than_prev_move);
	cy_run_test(move_chosen_in_an_unoccupied_place);

	cy_display_result();
	cy_return_result();
}

cy_new_test(check_graph_is_NULL_after_finalize) {
	struct graph_t *g = create_graph('c', 6);

	initialize_graph(g);

	finalize();

	cy_assert(p.player_graph == NULL, "Player graph is now NULL after finalize...");

	p.player_graph = NULL;
	cy_return_result();
}

cy_new_test(test_finalize) {
	cy_run_test(check_graph_is_NULL_after_finalize);
	cy_display_result();
	cy_return_result();
}

cy_new_test(test_random_player) {
	srand(time(NULL));
	cy_announce_test("Testing file random_player");
	cy_run_test(test_get_player_name);
	cy_run_test(test_initialize_graph);
	cy_run_test(test_initialize_color);
	cy_run_test(test_propose_opening);
	cy_run_test(test_accept_opening);
	cy_run_test(test_play);
	cy_run_test(test_finalize);
	cy_display_result();
	cy_return_result();
}
