#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_spmatrix.h>
#include "graph.h"
#include "graph_manager.h"
#include "../lib/set-src/include/set.h"

/////  UTILITIES  //////////////////////////////////////////////////////////////////////////////

/**
 * \fn struct set *same_colored_vertices(struct graph_t *game_table, int color)
 * \brief construct a set of same colored vertices in the board.
 * \param game_table game board from where we get the vertices
 * \param color the color of vertices
 * \return A set of same colored vertices in the board.
 */
struct set *same_colored_vertices(struct graph_t *game_table, int color) {
  struct set *list = set__empty();
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    if (gsl_spmatrix_get(game_table->o,color,i)) {
      set__add(list,i);
    }
  }
  return list;
}

/**
 * \fn int is_square(struct graph_t *game_table)
 * \brief predicate that verify the shape of the game board
 * \param game_table the graph of game table
 * \return (boolean) 1 if game table is squared or hexagonal 0 else
 */
int is_square(struct graph_t *game_table) {
  size_t num = game_table->num_vertices;
  size_t sqr = (size_t) sqrt(num);
  return (sqr * sqr == num);
}

size_t graph_side_size(struct graph_t *game_table) {
  size_t num = game_table->num_vertices;
  if (is_square(game_table)) {
    return (size_t) sqrt(num);  
  }
  else {
    return (size_t) sqrt(num/6);
  }
}

/**
 * \fn int search_start(struct graph_t *game_table, int color)
 * \brief search the first colored (with color ) vertex in the board
 * \param game_table the board where we search the start
 * \param color the color of the searched vertex
 * \return the vertex searched
 */
int search_start(struct graph_t *game_table, int color) {
  int start;
  int side_size = graph_side_size(game_table);
  if (is_square(game_table)) {
    start = color == 0 ? 0 : side_size - 1;
  }
  else {
    start = color == 0 ? 1 : 0;
  }
  return start;
}

/**
 * \fn int search_end(struct graph_t *game_table, int color)
 * \brief search the last colored (with color ) vertex in the board
 * \param game_table the board where we search the end
 * \param color the color of the searched vertex
 * \return the vertex searched
 */
int search_end(struct graph_t *game_table, int color) {
  int end;
  int side_size = graph_side_size(game_table);
  if (is_square(game_table)) {
    end = color == 0 ? game_table->num_vertices - 1 : game_table->num_vertices - side_size;
  }
  else {
    end = color == 0 ? 6 * side_size - 2 : 6 * side_size - 3;
  }
  return end;
}

int is_edge(struct graph_t *game_table, int v1, int v2) {
  return (gsl_spmatrix_get(game_table->t,v1,v2) == 1);
}

int can_keep_playing(struct graph_t *game_table) {
  for (int i = 0 ; i < game_table->num_vertices ; i++) {
    int bv = gsl_spmatrix_get(game_table->o,0,i);
    int wv = gsl_spmatrix_get(game_table->o,1,i);
    if ( (bv == 0) && (wv == 0) ) {
      return 1;
    }
  }
  return 0;
}

void update_graph(struct graph_t *graph, struct move_t move) {
	gsl_spmatrix_set(graph->o, move.c, move.m, 1);
}

int get_vertex_value(struct graph_t *graph, enum color_t row_i, int column_i) {
	return gsl_spmatrix_get(graph->o, row_i, column_i);
}

struct graph_t *copy_graph(struct graph_t *graph) {
  struct graph_t *new_graph = (struct graph_t *) malloc(sizeof(struct graph_t));
  new_graph->num_vertices = graph->num_vertices;
  new_graph->t = gsl_spmatrix_alloc(new_graph->num_vertices, new_graph->num_vertices);
  new_graph->o = gsl_spmatrix_alloc(2, new_graph->num_vertices);
  for (int i = 0; i < new_graph->num_vertices; i++) {
    if (gsl_spmatrix_get(graph->o, 0, i) == 1) {
      gsl_spmatrix_set(new_graph->o, 0, i, 1);
    }
    if (gsl_spmatrix_get(graph->o, 1, i) == 1) {
      gsl_spmatrix_set(new_graph->o, 1, i, 1);
    }
    for (int j = 0; j < new_graph->num_vertices; j++ ) {
      if (gsl_spmatrix_get(graph->t, i ,j) == 1) {
        gsl_spmatrix_set(new_graph->t, i , j, 1);
      }
    }
  }
  return new_graph;
}

/////  GRAPH CREATION  /////////////////////////////////////////////////////////////////////////

/**
 * \fn struct graph_t *create_square_graph(size_t side_size)
 * \brief creates a squared graph
 * \param side_size the side size of the graph
 * \return a squared graph
 */
struct graph_t *create_square_graph(size_t side_size) {
  struct graph_t *game_table = (struct graph_t*) malloc(sizeof(struct graph_t));
  game_table->num_vertices = side_size * side_size;
  game_table->t = gsl_spmatrix_alloc(game_table->num_vertices, game_table->num_vertices);
  game_table->o = gsl_spmatrix_alloc(2, game_table->num_vertices);

  for (int i = 0 ; i < side_size * (side_size - 1) ; i++) {
    gsl_spmatrix_set(game_table->t, i, i + side_size, 1);
    gsl_spmatrix_set(game_table->t, i + side_size, i, 1);
  }
  for (int i = 0 ; i < game_table->num_vertices - 1 ; i++) {
    if (i % side_size != side_size - 1) {
      gsl_spmatrix_set(game_table->t, i, i + 1, 1);
      gsl_spmatrix_set(game_table->t, i + 1, i, 1);
    }
  }

  for (int i = 0 ; i < side_size - 1 ; i++) {
    gsl_spmatrix_set(game_table->o, 0, i, 1);
    gsl_spmatrix_set(game_table->o, 0, game_table->num_vertices - i - 1, 1);
    gsl_spmatrix_set(game_table->o, 1, side_size * (i + 1) - 1, 1);
    gsl_spmatrix_set(game_table->o, 1, side_size * (i + 1), 1);
  }

  return game_table;
}

/**
 * \fn struct graph_t *create_hex_graph(size_t side_size)
 * \brief creates a hex graph
 * \param side_size the side size of the graph
 * \return a hex graph
 */
struct graph_t *create_hex_graph(size_t side_size) {
  struct graph_t *game_table = create_square_graph(side_size);
  for (int i = 0 ; i < side_size * (side_size - 1) ; i++) {
    if (i % side_size != 0) {
      gsl_spmatrix_set(game_table->t, i, i + side_size - 1, 1);
      gsl_spmatrix_set(game_table->t, i + side_size - 1, i, 1);
    }
  }

  return game_table;
}

/**
 * \fn struct graph_t *create_tri_graph(size_t side_size)
 * \brief creates a triagular graph
 * \param side_size the side size of the graph
 * \return a triagular graph
 */
struct graph_t *create_tri_graph(size_t side_size) {
  struct graph_t *game_table = (struct graph_t*) malloc(sizeof(struct graph_t));
  game_table->num_vertices = 6 * side_size * side_size;
  game_table->t = gsl_spmatrix_alloc(game_table->num_vertices, game_table->num_vertices);
  game_table->o = gsl_spmatrix_alloc(2, game_table->num_vertices);

  int i = (0) - 3;
  int j = (6 * (2 * side_size - 1) - 1) - 1;

  for (int spiral_count = 0 ; spiral_count < game_table->num_vertices - 1 ; spiral_count++) {
    gsl_spmatrix_set(game_table->t, spiral_count, spiral_count + 1, 1);
    gsl_spmatrix_set(game_table->t, spiral_count + 1, spiral_count, 1);
  } 
  
  for (int depth_count = side_size ; depth_count > 1 ; depth_count--) {
    i = (i + 3) - 2;
    j = (j + 1) - 2;
    for (int a = 0 ; a < depth_count ; a++) {
      i = i + 2;
      j = j + 2;
      gsl_spmatrix_set(game_table->t, i, j, 1);
      gsl_spmatrix_set(game_table->t, j, i, 1);
    }
    for (int b = 0 ; b < 4 ; b++) {
      i = (i + 3) - 2;
      j = (j + 1) - 2;
      for (int c = 0 ; c < depth_count - 1 ; c++) {
	i = i + 2;
	j = j + 2;
	gsl_spmatrix_set(game_table->t, i, j, 1);
	gsl_spmatrix_set(game_table->t, j, i, 1);
      }
    }
    i = (i + 3) - 2;
    j = (j + 1) - 2;
    for (int d = 0 ; d < depth_count - 2 ; d++) {
      i = i + 2;
      j = j + 2;
      gsl_spmatrix_set(game_table->t, i, j, 1);
      gsl_spmatrix_set(game_table->t, j, i, 1);
    }
  }
  i = (i + 4) + 2 - 3;
  j = (j + 0) + 2 - 1;
  gsl_spmatrix_set(game_table->t, i, j, 1);
  gsl_spmatrix_set(game_table->t, j, i, 1);

  int even = 2 * (side_size + (side_size - 2) / 2);
  int odd = 2 * (side_size + (side_size - 3) / 2) + 1;
  int colored_line = side_size % 2 == 0 ? even : odd;
  
  int start_1 = 1;
  int start_2 = colored_line + 2;
  int start_3 = 2 * colored_line + 2;
  int start_4 = 3 * colored_line + 3;
  
  for(int k = 0 ; k < colored_line ; k++) {
    gsl_spmatrix_set(game_table->o, 0, k + start_1, 1);
    gsl_spmatrix_set(game_table->o, 1, k + start_2, 1);
    gsl_spmatrix_set(game_table->o, 0, k + start_3, 1);
    gsl_spmatrix_set(game_table->o, 1, k + start_4, 1);
  }
  if (side_size % 2 != 0) {
    gsl_spmatrix_set(game_table->o, 0, colored_line + 1, 1);
    gsl_spmatrix_set(game_table->o, 0, 3 * colored_line + 2, 1);
  }
  gsl_spmatrix_set(game_table->o, 1, 0, 1);
  gsl_spmatrix_set(game_table->o, 1, start_4 + colored_line - 1, 0);
  /*
  for (int p = 0 ; p < game_table->num_vertices ; p++) {
    printf("%d %f %f\n",p ,gsl_spmatrix_get(game_table->o,0,p),gsl_spmatrix_get(game_table->o,1,p));
  } 
  */
  return game_table;
}

struct graph_t *create_graph(char table_type, int table_size) {
  struct graph_t *game_table;
  switch (table_type) {
  case 'h':
    game_table = create_hex_graph(table_size + 1);
    break;
  case 'c':
    game_table = create_square_graph(table_size + 1);
    break;  
  case 't' :
    game_table = create_tri_graph(table_size);
    break;
  default :
    game_table = NULL;
    break;
  }

  return game_table;
}

void free_graph(struct graph_t *g) {
  gsl_spmatrix_free(g->t);
  g->t = NULL;
  gsl_spmatrix_free(g->o);
  g->o = NULL;
  free(g);
}

/////  WIN CHECK  //////////////////////////////////////////////////////////////////////////////

struct set *can_visit(struct graph_t *game_table, int v, struct set *visitable) {
  struct set *list = set__empty();
  for (int count = 0; count < set__size(visitable); count++) {
    if (is_edge(game_table,v,set__value(visitable,count))) {
      set__add(list,set__value(visitable,count));
    }
  }
  return list;
}

int is_linked(struct graph_t *game_table, int v1, int v2, struct set *visitable) {
  if (v1 == v2) {
    return 1;
  }
  
  set__remove(visitable,v1);
  struct set *possibles = can_visit(game_table, v1, visitable);
  while (!set__is_empty(possibles)) {
    int next = set__random_value(possibles); 
    if (is_linked(game_table, next, v2, visitable)) {
      set__free(possibles);
      return 1;
    }
    else {
      set__remove(possibles,next);
    }
  }
  set__free(possibles);
  return 0;
}

int color_is_winning(struct graph_t *game_table, int color) {
  struct set *visitable = same_colored_vertices(game_table, color);
  /*
  int side_size = graph_side_size(game_table);
  int start = color == 0 ? 0 : side_size - 1;
  int end = color == 0 ? game_table->num_vertices - 1 : game_table->num_vertices - side_size;
  */
  int start = search_start(game_table, color);
  int end = search_end(game_table, color);
  int is_win = is_linked(game_table, start, end, visitable);
  set__free(visitable);
  return is_win;
}
