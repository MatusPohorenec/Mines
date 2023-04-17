#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "ui.h"
#include "board.h"
#include "view.h"
#include "str_utils.h"

#define HOF_FILENAME "score.txt"

/* private functions */
void game_loop(Game *game);
void print_play_field(Game *game, int input_row, int input_column);
void print_score(Player *list_of_players, int number_of_all_players);
void print_input_rules();

/**
 * Ask a player for his name and store in the game.
 */
void read_player_name(Game *game) {
    assert(game != NULL);
    printf("Zadajte vaše meno prosím: \n");
    char name_of_player[MAX_PLAYER_LENGTH];
    fgets(name_of_player, MAX_PLAYER_LENGTH, stdin);
    str_remove_spaces(name_of_player);
    strcpy(game->player->name, name_of_player);
}

/**
 * Handle whole process of the Game
 */
void play_game(Game *game) {
    assert(game != NULL);

    Player list_of_players[PLAYERS_LIMIT];
    int size_of_list = read_scores(HOF_FILENAME, list_of_players);
    if (size_of_list > 0) {
        print_score(list_of_players, size_of_list);
    }
    
    //generate_mines_after_first_move(game->board);
    
    game_loop(game);

    if (game->game_state == SOLVED) {
        printf("Gratulujem %s. Riešenie je správne!\n", game->player->name);
    } else {
        printf("Ľutujem %s. Riešenie je nesprávne!\n", game->player->name);
    }
    printf("Vaše skóre je: %d\n", game->player->score);

    bool is_player_added_to_list =
            add_player_to_list(list_of_players, &size_of_list, *game->player);

    if (is_player_added_to_list) {
        save_scores(HOF_FILENAME, list_of_players, size_of_list);
    }
}

/**
 * Handles players input process
 * Loops until game state is playing
 */
void game_loop(Game *game) {
    assert(game != NULL);
    int input, input_row = -1, input_column = -1;
    int move = 0;
    char type;

    //first move in the game 
    print_play_field(game, input_row, input_column);
    print_input_rules();
    input = scanf("%d %d", &input_row, &input_column);
    //generate the mines after the first move exept for the entred first possition  
    generate_mines_after_first_move(game->board, input_row - 1, input_column - 1);

    if (input != EOF) {
            // if input is not empty or is not correct then open tile
            open_tile(game, input_row - 1, input_column - 1,'o');
    }
    if(game->game_state == FAILED){
        return;
    }
    do {
        print_play_field(game, input_row, input_column);
        print_input_rules();
        scanf("%c %d %d", &type, &input_row, &input_column);
        while (getchar() != '\n');
        while (true)
        {
            if ((type == 'm' || type == 'o') && input_column <= 30 && input_row <= 30)
            {
                break;
            }
            print_input_rules();
            scanf("%c %d %d", &type, &input_row, &input_column);
            while (getchar() != '\n');
        }
        move++;
        open_tile(game, input_row - 1, input_column - 1, type);
    } while (game->game_state == PLAYING);

    print_play_field(game, input_row, input_column);
}

void print_score(Player *list_of_players, int number_of_all_players) {
    char *text = view_hof(list_of_players, number_of_all_players);
    printf("%s", text);
    free(text);
}

void print_play_field(Game *game, int input_row, int input_column) {
    char *field = view_play_field(game->board, input_row, input_column);
    printf("\n%s\n", field);
    free(field);
}

void print_input_rules() {
    printf("Zadajte pismeno o/m (o = odkrytie policka, m = oznacenie), číslo riadka, medzeru a číslo stĺpca. Napr. o 2 3, m 5 9\n");
}
