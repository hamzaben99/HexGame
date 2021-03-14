#include <stdio.h>
#include <stdlib.h>

#include "test_random_player.h"
#include "test_player_loader.h"
#include "test_graph_generator.h"
#include "test_set.h"
#include "test_connex_player.h"
#include "cyounit.h"

int main(void) {
	cy_init_testing();
	cy_announce_test("Testing Hex-9150");
	
	#ifdef CONNEX
	cy_run_test(test_connex_player);
	#elif RANDOM
	cy_run_test(test_random_player);
	#elif PROJECT
	cy_run_test(test_player_loader);
	cy_run_test(test_graph_generator);
	cy_run_test(test_set);
    #endif

	cy_display_result();
	cy_return_result();
}
