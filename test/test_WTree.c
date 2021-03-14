#include "cyounit.h"
#include "../src/WTree.h"
#include "test_WTree.h"
#include "graph.h"
#include "graph_manager.h"

cy_new_test(initialization) {
    WTree *t = init_tree(5, 16, 20);
    cy_assert(t->start == 5 && t->end == 16 && t->num_vertices == 20,"strat, end and num_vertices members well initialized...");
    tree_free(t);
    cy_return_result();
}

cy_new_test(test_init_tree) {
    cy_announce_test("Testing init_tree function");
    cy_run_test(initialization);
    cy_display_result();
    cy_return_result();
}

cy_new_test(same_colored) {
    struct graph_t *g = create_graph('h', 3);
    WTree *t = init_tree(0, 15, 16);
    init_weights(g, t, BLACK);
    cy_assert(fabs(t->weights[0][1] - 0.f) <= 0.1 && fabs(t->weights[1][2] - 0.f) <= 0.1 &&
              fabs(t->weights[15][14] - 0.f) <= 0.1 && fabs(t->weights[14][13] - 0.f) <= 0.1, "Weights between same colored vertices...");
    free_graph(g);
    tree_free(t);
    cy_return_result();
}

cy_new_test(opposite_colored) {
    struct graph_t *g = create_graph('h', 3);
    WTree *t = init_tree(0, 15, 16);
    init_weights(g, t, BLACK);
    cy_assert(isinf(t->weights[2][3])  && isinf(t->weights[0][4])  &&
              isinf(t->weights[12][13])  && isinf(t->weights[15][11])  &&
              isinf(t->weights[1][4])  && isinf(t->weights[14][11]), "Weights between opposite colored vertices...");
    free_graph(g);
    tree_free(t);
    cy_return_result();
}

cy_new_test(none_colored) {
    struct graph_t *g = create_graph('h', 3);
    WTree *t = init_tree(0, 15, 16);
    init_weights(g, t, BLACK);
    cy_assert(fabs(t->weights[5][6] - 1.f) <= 0.1 && fabs(t->weights[9][10] - 1.f) <= 0.1 &&
              fabs(t->weights[5][9] - 1.f) <= 0.1 && fabs(t->weights[10][6] - 1.f) <= 0.1 &&
              fabs(t->weights[9][6] - 1.f) <= 0.1, "Weights between none colored vertices...");
    free_graph(g);
    tree_free(t);
    cy_return_result();
}

cy_new_test(semi_colored) {
    struct graph_t *g = create_graph('h', 3);
    WTree *t = init_tree(0, 15, 16);
    init_weights(g, t, BLACK);
    cy_assert(fabs(t->weights[1][5] - 0.5f) <= 0.1 && fabs(t->weights[13][9] - 0.5f) <= 0.1 &&
              fabs(t->weights[2][6] - 0.5f) <= 0.1 && fabs(t->weights[14][10] - 0.5f) <= 0.1 &&
              fabs(t->weights[2][5] - 0.5f) <= 0.1 && fabs(t->weights[13][10] - 0.5f) <= 0.1, "Weights between none colored and colored vertices...");
    free_graph(g);
    tree_free(t);
    cy_return_result();
}

cy_new_test(test_init_weights) {
    cy_announce_test("Testing init_weights function");
    cy_run_test(same_colored);
    cy_run_test(opposite_colored);
    cy_run_test(none_colored);
    cy_run_test(semi_colored);
    cy_display_result();
    cy_return_result();
}

cy_new_test(update_after_move) {
    struct graph_t *g = create_graph('h', 3);
    WTree *t_p = init_tree(0, 14, 16);
    WTree *t_np = init_tree(4, 11, 16);
    struct move_t move = {.c = BLACK , .m = 5};
    init_weights(g, t_p, BLACK);
    init_weights(g, t_np, WHITE);
    update_graph(g, move);
    update_weights(g, t_p, t_np, move);
    cy_assert(fabs(t_p->weights[1][5] - 0.f) <= 0.1 && fabs(t_p->weights[2][5] - 0.f) <= 0.1 &&
              fabs(t_p->weights[5][9] - 0.5f) <= 0.1 && fabs(t_p->weights[6][5] - 0.5f) <= 0.1 &&
              isinf(t_np->weights[5][4]) && isinf(t_np->weights[5][8]), "Update weights after a move...");
    free_graph(g);
    tree_free(t_p);
    tree_free(t_np);
    cy_return_result();
}

cy_new_test(test_update_weights) {
    cy_announce_test("Testing update_weights function");
    cy_run_test(update_after_move);
    cy_display_result();
    cy_return_result();
}

cy_new_test(test_WTree) {
    cy_announce_test("Testing WTree functions");
    cy_run_test(test_init_tree);
    cy_run_test(test_init_weights);
    cy_run_test(test_update_weights);
    cy_display_result();
    cy_return_result();
}