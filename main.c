#include <stdlib.h>
#include "game.h"
#include "ui.h"
#include "board.h"

int main() {
    Game *game = create_game();
    Board *board = create_board(9, 9, 9);
    game->board = board;
    read_player_name(game);
    play_game(game);
    destroy_game(game);
    exit(EXIT_SUCCESS);
}
