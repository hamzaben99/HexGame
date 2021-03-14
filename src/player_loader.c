#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "player_loader.h"

/**
 * \fn void check_failure(void)
 * \brief Check if there is a failure when getting a symbol.
 */
void check_failure(void) {
	char *error;
	if((error = dlerror()) != NULL) {
		fputs(error, stderr);
		exit(EXIT_FAILURE);
	}
}

struct player initialize_player(char *lib_path) {
	void *handler = dlopen(lib_path, RTLD_NOW);

	if(handler == NULL) {
		fputs(dlerror(), stderr);
		exit(EXIT_FAILURE);
	}
	struct player p;
	p.handler = handler;
	p.get_player_name = dlsym(handler, "get_player_name");
    check_failure();
	p.propose_opening = dlsym(handler, "propose_opening");
    check_failure();
	p.accept_opening = dlsym(handler, "accept_opening");
    check_failure();
	p.initialize_graph = dlsym(handler, "initialize_graph");
    check_failure();
	p.initialize_color = dlsym(handler, "initialize_color");
    check_failure();
	p.play = dlsym(handler, "play");
    check_failure();
	p.finalize = dlsym(handler, "finalize");
    check_failure();

	return p;
}

int close_player(struct player p) {
	int n = dlclose(p.handler);
	return n;
}
