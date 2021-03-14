#include "cyounit.h"
#include "../src/methods.h"
#include "test_methods.h"
#include "WTree.h"
#include "graph.h"
#include "graph_manager.h"
#include "../lib/set-src/include/set.h"

struct set *construct_path_test(void (* meth) (struct graph_t *, WTree *)) {
    struct graph_t *g = create_graph('c', 4);
    WTree *t_b = init_tree(0, 24, 25);
    WTree *t_w = init_tree(5, 19, 25);
    struct set *path = set__empty();
    init_weights(g, t_b, BLACK);
    init_weights(g, t_w, WHITE);
    for (int i = 0; i < 3; i++) {
        struct move_t m1 = {.c = WHITE, .m = 5 * i + 6};
        struct move_t m2 = {.c = WHITE, .m = 5 * i + 9};
        update_weights(g, t_w, t_b, m1);
        update_weights(g, t_w, t_b, m2);
    }
    meth(g, t_b);
    int size = set__size(t_b->path);
    for (int i = 0; i < size; i++) {
        set__add(path, set__value(t_b->path, i));
    }
    free_graph(g);
    tree_free(t_b);
    tree_free(t_w);
    return path;
}

cy_new_test(test_Dijkstra_path) {
    struct set *path = construct_path_test(Dijkstra);
    int size = set__size(path);
    int res = 0;
    for (int i = 0, j = 0; i < size; i++) {
        if (set__value(path, i) == 1 || set__value(path, i) == 2 || set__value(path, i) == 24) {
            res ++;
        }
        if (set__value(path, i) == 5 * j + 3) {
            j ++;
            res ++;
        }
    }
    set__free(path);
    cy_assert(res == size, "check shortest path returned by Dijkstra's algorithm...");
    cy_return_result();
}

cy_new_test(test_Dijkstra) {
    cy_announce_test("Testing Dijkstra's algorithm");
    cy_run_test(test_Dijkstra_path);
    cy_display_result();
    cy_return_result();
}

cy_new_test(test_Bellman_Ford_path) {
    struct set *path = construct_path_test(Bellman_Ford);
    int size = set__size(path);
    int res = 0;
    for (int i = 0, j = 0; i < size; i++) {
        if (set__value(path, i) == 1 || set__value(path, i) == 2 || set__value(path, i) == 24) {
            res ++;
        }
        if (set__value(path, i) == 5 * j + 3) {
            j ++;
            res ++;
        }
    }
    set__free(path);
    cy_assert(res == size, "check shortest path returned by Bellman-Ford's algorithm...");
    cy_return_result();
}

cy_new_test(test_Bellman_Ford) {
    cy_announce_test("Testing Bellman-Ford's algorithm");
    cy_run_test(test_Bellman_Ford_path);
    cy_display_result();
    cy_return_result();
}

cy_new_test(test_FLoyd_Warshall_path) {
    struct set *path = construct_path_test(FLoyd_Warshall);
    int size = set__size(path);
    int res = 0;
    for (int i = 0, j = 0; i < size; i++) {
        if (set__value(path, i) == 1 || set__value(path, i) == 2 || set__value(path, i) == 24) {
            res ++;
        }
        if (set__value(path, i) == 5 * j + 3) {
            j ++;
            res ++;
        }
    }
    set__free(path);
    cy_assert(res == size , "check shortest path returned by FLoyd-Warshall's algorithm...");
    cy_return_result();
}

cy_new_test(test_FLoyd_Warshall) {
    cy_announce_test("Testing FLoyd-Warshall's algorithm");
    cy_run_test(test_FLoyd_Warshall_path);
    cy_display_result();
    cy_return_result();
}

cy_new_test(test_methods) {
    cy_announce_test("Testing methods functions");
    cy_run_test(test_Dijkstra);
    cy_run_test(test_Bellman_Ford);
    cy_run_test(test_FLoyd_Warshall);
    cy_display_result();
    cy_return_result();
}