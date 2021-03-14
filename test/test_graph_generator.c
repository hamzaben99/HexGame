#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_spmatrix.h>
#include <time.h>

#include "../src/graph.h"
#include "test_graph_generator.h"
#include "cyounit.h"
#include "graph_manager.h"

extern struct graph_t *player_graph;
extern enum color_t player_color;

cy_new_test(square_shaped_graph_is_well_created) {
  
  int player_side_size = 7;
  struct graph_t *game_table = create_graph('c',player_side_size);
  cy_assert(game_table->num_vertices == (player_side_size + 1) * (player_side_size + 1), "square graph creation generates the right number of vertexes...");
  int side_size = graph_side_size(game_table);
  
  int one_count = 0;
  int zero_count = 0;
  int other_count = 0;
  int right_place_one_count = 0;
  int expected_one_count = 4 * side_size * (side_size - 1);
  
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    for (int j = 0 ; j < game_table->num_vertices ; j++) {
      int value = gsl_spmatrix_get(game_table->t,i,j);
      switch (value) {
      case 0:
	zero_count++;
	break;
      case 1:
	one_count++;
	break;  
      default:
	other_count++;
	break;
      }
    }
  }
  
  for (int i = 0 ; i < side_size * (side_size - 1) ; i++) {
    if(gsl_spmatrix_get(game_table->t, i, i + side_size) == 1)
      right_place_one_count ++;
    if (gsl_spmatrix_get(game_table->t, i + side_size, i) == 1)
      right_place_one_count ++;
  }
  
  for (int i = 0 ; i < game_table->num_vertices - 1 ; i++) {
    if (i % side_size != side_size - 1) {
      if (gsl_spmatrix_get(game_table->t, i, i + 1) == 1)
	right_place_one_count ++;
      if (gsl_spmatrix_get(game_table->t, i + 1, i) == 1)
	right_place_one_count ++;
    }
  }

  cy_assert(expected_one_count == one_count, "square graph creation makes the right number of edges...");
  cy_assert((game_table->num_vertices) * (game_table->num_vertices) == (one_count + zero_count), "square graph creation does not generate aberrant values...");
  cy_assert(expected_one_count == right_place_one_count, "square graph creation makes edges in the right places...");

  int black_count = 0;
  int white_count = 0;
  int blank_count = 0;
  other_count = 0;
  int right_place_black_count = 0;
  int right_place_white_count = 0;
  int expected_count = 2 * (side_size - 1);
  
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    int bv = gsl_spmatrix_get(game_table->o,0,i);
    int wv = gsl_spmatrix_get(game_table->o,1,i);
    int value = bv + 2 * wv;
    switch (value) {
    case 0:
      blank_count++;
      break;
    case 1:
      black_count++;
      break;
    case 2:
      white_count++;
      break;  
    default:
      other_count++;
      break;
    }
  }
  
  for (int i = 0 ; i < side_size - 1 ; i++) {
    if (gsl_spmatrix_get(game_table->o, 0, i) == 1)
      right_place_black_count += 1;
    if (gsl_spmatrix_get(game_table->o, 0, game_table->num_vertices - i - 1) == 1)
      right_place_black_count += 1;
    if (gsl_spmatrix_get(game_table->o, 1, side_size * (i + 1)) == 1)
      right_place_white_count += 1;
    if (gsl_spmatrix_get(game_table->o, 1, side_size * (i + 1) + 1) == 1)
      right_place_white_count += 1;
  }
  cy_assert(expected_count == black_count, "square graph creation makes the right number of black vertexes...");
  cy_assert(expected_count == black_count, "square graph creation colors the right vertexes black...");
  cy_assert(expected_count == white_count, "square graph creation makes the right number of white vertexes...");
  cy_assert(expected_count == black_count, "square graph creation colors the right vertexes white...");
  cy_assert( game_table->num_vertices == black_count + white_count + blank_count , "square graph creation does not generate aberrant colors...")

  free_graph(game_table);
  
  cy_return_result();
}

cy_new_test(hex_shaped_graph_is_well_created) {

  int player_side_size = 7;
  struct graph_t *game_table = create_graph('h',player_side_size);
  cy_assert(game_table->num_vertices == (player_side_size + 1) * (player_side_size + 1), "hex graph creation generates the right number of vertexes...");
  int side_size = graph_side_size(game_table);
  
  int one_count = 0;
  int zero_count = 0;
  int other_count = 0;
  int right_place_one_count = 0;
  int expected_one_count = 4 * side_size * (side_size - 1) + 2 * (side_size - 1) * (side_size - 1) ;
  
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    for (int j = 0 ; j < game_table->num_vertices ; j++) {
      int value = gsl_spmatrix_get(game_table->t,i,j);
      switch (value) {
      case 0:
	zero_count++;
	break;
      case 1:
	one_count++;
	break;  
      default:
	other_count++;
	break;
      }
    }
  }
  
  for (int i = 0 ; i < side_size * (side_size - 1) ; i++) {
    if(gsl_spmatrix_get(game_table->t, i, i + side_size) == 1)
      right_place_one_count ++;
    if (gsl_spmatrix_get(game_table->t, i + side_size, i) == 1)
      right_place_one_count ++;
  }
  
  for (int i = 0 ; i < game_table->num_vertices - 1 ; i++) {
    if (i % side_size != side_size - 1) {
      if (gsl_spmatrix_get(game_table->t, i, i + 1) == 1)
	right_place_one_count ++;
      if (gsl_spmatrix_get(game_table->t, i + 1, i) == 1)
	right_place_one_count ++;
    }
  }
  
  for (int i = 0 ; i < side_size * (side_size - 1) ; i++) {
    if (i % side_size != 0) {
      if (gsl_spmatrix_get(game_table->t, i, i + side_size - 1) == 1)
	right_place_one_count ++;
      if (gsl_spmatrix_get(game_table->t, i + side_size - 1, i) == 1)
	right_place_one_count ++;
    }
  }

  cy_assert(expected_one_count == one_count, "hex graph creation makes the right number of edges...");
  cy_assert((game_table->num_vertices) * (game_table->num_vertices) == (one_count + zero_count), "hex graph creation does not generate aberrant values...");
  cy_assert(expected_one_count == right_place_one_count, "hex graph creation makes edges in the right places...");

  int black_count = 0;
  int white_count = 0;
  int blank_count = 0;
  other_count = 0;
  int right_place_black_count = 0;
  int right_place_white_count = 0;
  int expected_count = 2 * (side_size - 1);
  
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    int bv = gsl_spmatrix_get(game_table->o,0,i);
    int wv = gsl_spmatrix_get(game_table->o,1,i);
    int value = bv + 2 * wv;
    switch (value) {
    case 0:
      blank_count++;
      break;
    case 1:
      black_count++;
      break;
    case 2:
      white_count++;
      break;  
    default:
      other_count++;
      break;
    }
  }
  
  for (int i = 0 ; i < side_size - 1 ; i++) {
    if (gsl_spmatrix_get(game_table->o, 0, i) == 1)
      right_place_black_count += 1;
    if (gsl_spmatrix_get(game_table->o, 0, game_table->num_vertices - i - 1) == 1)
      right_place_black_count += 1;
    if (gsl_spmatrix_get(game_table->o, 1, side_size * (i + 1)) == 1)
      right_place_white_count += 1;
    if (gsl_spmatrix_get(game_table->o, 1, side_size * (i + 1) + 1) == 1)
      right_place_white_count += 1;
  }
  
  cy_assert(expected_count == black_count, "hex graph creation makes the right number of black vertexes...");
  cy_assert(expected_count == black_count, "hex graph creation colors the right vertexes black...");
  cy_assert(expected_count == white_count, "hex graph creation makes the right number of white vertexes...");
  cy_assert(expected_count == black_count, "hex graph creation colors the right vertexes white...");
  cy_assert( game_table->num_vertices == black_count + white_count + blank_count , "hex graph creation does not generate aberrant colors...")

  free_graph(game_table);
  
  cy_return_result();
}

cy_new_test(test_create_graph) {
	cy_announce_test("Testing function create_graph")
	cy_run_test(square_shaped_graph_is_well_created);
	cy_run_test(hex_shaped_graph_is_well_created); 
	cy_display_result();
	cy_return_result();
}

cy_new_test(return_true_when_black_is_winning) {
	struct graph_t *g = create_graph('c', 3);

	gsl_spmatrix_set(g->o, 0, 5, 1.0);
	gsl_spmatrix_set(g->o, 0, 6, 1.0);
	gsl_spmatrix_set(g->o, 0, 9, 1.0);
	gsl_spmatrix_set(g->o, 0, 10, 1.0);
	
	cy_assert(color_is_winning(g, 0) == 1, "Should return true with a winner graph for player Black");
	free_graph(g);
	cy_return_result();
}

cy_new_test(return_true_when_white_is_winning) {
	struct graph_t *g = create_graph('c', 3);
	
	gsl_spmatrix_set(g->o, 1, 5, 1.0);
	gsl_spmatrix_set(g->o, 1, 6, 1.0);
	gsl_spmatrix_set(g->o, 1, 9, 1.0);
	gsl_spmatrix_set(g->o, 1, 10, 1.0);
	
	cy_assert(color_is_winning(g, 1) == 1, "Should return true with a winner graph for player White");
	free_graph(g);
	cy_return_result();
}

cy_new_test(return_false_when_the_other_color_is_winning) {
	struct graph_t *g = create_graph('c', 3);

	gsl_spmatrix_set(g->o, 1, 5, 1.0);
	gsl_spmatrix_set(g->o, 1, 6, 1.0);
	gsl_spmatrix_set(g->o, 1, 9, 1.0);
	gsl_spmatrix_set(g->o, 1, 10, 1.0);
	
	cy_assert(color_is_winning(g, 0) == 0, "Should return false with a winner graph for player White and ask if Black is winning");
	free_graph(g);
	cy_return_result();
}

cy_new_test(return_false_when_color_is_not_winning) {
	struct graph_t *g = create_graph('c', 3);
	
	cy_assert(color_is_winning(g, 0) == 0, "Should return false with not connex graph for player Black");
	cy_assert(color_is_winning(g, 1) == 0, "Should return false with not connex graph for player White");
	free_graph(g);
	cy_return_result();
}

cy_new_test(test_color_is_winning) {
	cy_announce_test("Testing function color_is_winning");
	cy_run_test(return_true_when_black_is_winning);
	cy_run_test(return_true_when_white_is_winning);
	cy_run_test(return_false_when_the_other_color_is_winning);
	cy_run_test(return_false_when_color_is_not_winning);
	cy_display_result();
	cy_return_result();
}

cy_new_test(graph_is_correctly_updated) {
	struct graph_t *g = create_graph('h', 10);
	struct move_t m;
	m.c = BLACK;
	m.m = 15;
	update_graph(g, m);
	cy_assert(get_vertex_value(g, m.c, m.m) == 1, "The value of the graph at this position should be 1");
	free_graph(g);
	cy_return_result();
}

cy_new_test(test_graph_update) {
	cy_announce_test("Testing function graph_update");
	cy_run_test(graph_is_correctly_updated);
	cy_display_result();
	cy_return_result();
}

cy_new_test(return_the_correct_value) {
	struct graph_t *g = create_graph('h', 10);
	gsl_spmatrix_set(g->o, 1, 5, 1.0);
	cy_assert(get_vertex_value(g, 1, 5) == 1, "The returned value should be 1");
	free_graph(g);
	cy_return_result();	
}

cy_new_test(test_get_vertex_value) {
	cy_announce_test("Testing function get_vertex_value");
	cy_run_test(return_the_correct_value);
	cy_display_result();
	cy_return_result();
}

cy_new_test(test_graph_generator) {
	srand(time(NULL));
	cy_announce_test("Testing file graph (graph creation)");
	cy_run_test(test_create_graph);
	cy_run_test(test_color_is_winning);
	cy_run_test(test_get_vertex_value);
	cy_run_test(test_graph_update);
	cy_display_result();
	cy_return_result();
}
