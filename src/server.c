#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

#include "graph.h"
#include "graph_manager.h"
#include "move.h"
#include "player_loader.h"

#define NB_PLAYERS 2
/**
 * \def NB_PLAYERS 2
 * \brief The max numbers of player.
 */


/**
 * \def MAX_PATH (100+1)
 * \brief The number max of character in a path.
 */
#define MAX_PATH (100+1)

/**
 * \def MAX_NAME (50+1)
 * \brief The number max of character in the name of .so files.
 */
#define MAX_NAME (50+1)

struct graph_t *graph;
static int size = 7;
static char shape = 'c';
static char first_player_path[MAX_PATH + MAX_NAME - 1];
static char second_player_path[MAX_PATH + MAX_NAME - 1];
static char cwd[MAX_PATH];
struct player players[NB_PLAYERS];
int is_first_player_unloaded = 1;

/**
 * \fn void display_usage(char *filename)
 * \brief Display help to execute the program.
 * \param filename The name of the executable.
 */
void display_usage(char *filename) {
	fprintf(stderr, "Usage: %s [-m board size] [-t board shape] <player1> <player2>\n", filename);
}

/**
 * \fn void parse_opts(int argc, char *argv[])
 * \brief Parse the options and display error and help if the options are invalid.
 * \param argc The number of arguments in command line.
 * \param argv The value of the different arguments in command line.
 */ 
void parse_opts(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "m:t:")) != -1) {
		switch (opt) {
			case 'm':
				size = atoi(optarg);
				break;
			case 't':
				shape = *optarg;
				break;
			default:
				display_usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	if (argc - optind > 2) {
		fprintf(stderr, "Given too much arguments.\n");
		display_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc - optind < 2) {
		fprintf(stderr, "Given too few arguments.\n");
		display_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	for (; optind < argc; optind++) {
		if (is_first_player_unloaded) {
			strcat(first_player_path, cwd);
			strcat(first_player_path, argv[optind]);
			is_first_player_unloaded = 0;
		} else {
			strcat(second_player_path, cwd);
			strcat(second_player_path, argv[optind]);
		}
	}
	printf("First player: %s\nSecond player: %s\n", first_player_path, second_player_path);
}

/**
 * \fn void parser(int argc, char *argv[])
 * \brief Call the option parser and display error and help if the options are invalid.
 * \param argc The number of arguments in command line.
 * \param argv The value of the different arguments in command line.
 */ 
void parser(int argc, char *argv[]) {
	parse_opts(argc, argv);
	if (shape != 'c' && shape != 'h' && shape != 't') {
		printf("Please choose a valid shape [-t t], [-t h] or [-t c]\n");
		exit(EXIT_FAILURE);
	}
	if (size <= 1) {
		printf("Please choose a size greater than one\n");
		exit(EXIT_FAILURE);
	}
	if (argc <= 2) {
		printf("Please enter two player's paths\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * \fn void load_players()
 * \brief Call the function to initialize the 2 players.
 */
void load_players() {
	struct player first_player = initialize_player(first_player_path);
	printf("First player loaded\n");
	struct player second_player = initialize_player(second_player_path);
	printf("Second player loaded\n");
	players[0] = first_player;
	players[1] = second_player;
}

/**
 * \fn int is_winning(struct graph_t *g, enum color_t c)
 * \brief Check if one player win the game.
 * \return If the player is winning return 1, else 0.
 */
int is_winning(struct graph_t *g, enum color_t c) {
	return color_is_winning(g, c);
}

/**
 * \fn void announce_winner(enum color_t c, int winner_index)
 * \brief Display the winner
 * \param c Color of the winner (0 or 1), if no winner 2.
 * \param winner_index Index of the winner.
 */ 
void announce_winner(enum color_t c, int winner_index) {
	if (c == NO_COLOR || winner_index < 0 || winner_index > NB_PLAYERS) {
		printf("Ex aequo!\n");
	} else {
		printf("Player %s (%s) wins the game!\n",
				players[winner_index].get_player_name(),
				c == BLACK
				? "BLACK" : c == WHITE
				? "WHITE" : "NO_COLOR");
	}
}

/**
 * \fn run_game()
 * \brief Main loop of the game
 */
void run_game() {
	graph = create_graph(shape, size);
	
	/* Initialize players */
	load_players();
	for (int i = 0; i < NB_PLAYERS; ++i) {
		players[i].initialize_graph(create_graph(shape, size));
	}

	/* Opening */
	struct move_t move = players[0].propose_opening();
	printf("%s proposed to play at  %zu\n", players[0].get_player_name(), move.m);
	int players_index;
	if (players[1].accept_opening(move)) {
		players_index = 1;
		players[0].initialize_color(BLACK);
		players[1].initialize_color(WHITE);
		printf("%s accepted the opening\n", players[1].get_player_name());
	} else {
		players_index = 0;
		players[0].initialize_color(WHITE);
		players[1].initialize_color(BLACK);
		printf("%s refused the opening\n", players[1].get_player_name());
	}

	update_graph(graph, move);

	/* Main Loop */
	int continue_game = 1;
	while (continue_game) {
		move = players[players_index].play(move);
		update_graph(graph, move);
		printf("Player %s (%s) plays %zu\n",
				players[players_index].get_player_name(),
				move.c == BLACK
				? "BLACK" : move.c == WHITE
				? "WHITE" : "NO_COLOR",
		       move.m);

		/* Check for a winner */
		if (is_winning(graph, move.c)) {
			continue_game = 0;
			announce_winner(move.c, players_index);
		} else {
			/* Check for a tie game */
			if (!can_keep_playing(graph)) {
				continue_game = 0;
				announce_winner(2, -1);
			}
		}
		players_index = (players_index + 1) % NB_PLAYERS;
	}
	for (int i = 0; i < NB_PLAYERS; ++i) {
		players[i].finalize();
		close_player(players[i]);
	}
	free_graph(graph);
}

/**
 * \fn main(int argc, char *argv[])
 * \brief Main function of the program.
 */
int main(int argc, char *argv[]) {
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/"); // Add last '/' to avoid path issues
	parser(argc, argv);
	printf("Hex game - 9150\n");
	run_game();
	return EXIT_SUCCESS;
}
