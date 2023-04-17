#include "greatest.h"
#include "../board.h"
#include "../game.h"


TEST open_mine() {
    
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    srand(time(NULL));
    int first_move_row = generate_random_coordinates(game->board->row_count);
    int first_move_column = generate_random_coordinates(game->board->column_count);
    generate_mines_after_first_move(game->board,first_move_row,first_move_column);

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if(game->board->tiles[row][column]->is_mine){
                open_tile(game, row, column, 'o');
            }
        }
    }
    ASSERT_EQ(FAILED, game->game_state);
    destroy_game(game);
    PASS();
}

TEST first_move_without_mine() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    srand(time(NULL));
    int first_move_row = generate_random_coordinates(game->board->row_count);
    int first_move_column = generate_random_coordinates(game->board->column_count);
    generate_mines_after_first_move(game->board,first_move_row,first_move_column);

    if(game->board->tiles[first_move_row][first_move_column]->is_mine){
        open_tile(game, first_move_row, first_move_column,'o');
    }
    ASSERT_EQ(!FAILED, game->game_state);
    destroy_game(game);
    PASS();
}

TEST first_move_game_state_playing() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    srand(time(NULL));
    int first_move_row = generate_random_coordinates(game->board->row_count);
    int first_move_column = generate_random_coordinates(game->board->column_count);
    generate_mines_after_first_move(game->board,first_move_row,first_move_column);

    if(game->board->tiles[first_move_row][first_move_column]->is_mine){
        open_tile(game, first_move_row, first_move_column,'o');
    }
    ASSERT_EQ(PLAYING, game->game_state);
    destroy_game(game);
    PASS();
}


TEST open_tile_with_bad_coordinates() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;
    open_tile(game, -1, -1, 'o');
    open_tile(game, game->board->row_count, game->board->column_count, 'o');

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
                    ASSERT_EQ(CLOSED, game->board->tiles[row][column]->tile_state);
        }
    }
    destroy_game(game);
    PASS();
}


TEST amount_of_marked() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
                    open_tile(game, row, column, 'm');
        }
    }
    open_tile(game, 0,0, 'm');
    open_tile(game, 0,0, 'o');
    ASSERT_GTE(game->board->mine_count, game->marked_count);
    destroy_game(game);
    PASS();
}

TEST count_score_after_move() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 70);
    game->board = board;
    
    srand(time(NULL));
    int first_move_row = generate_random_coordinates(game->board->row_count);
    int first_move_column = generate_random_coordinates(game->board->column_count);
    generate_mines_after_first_move(game->board,first_move_row,first_move_column);

    bool is_closed_tile_open = false;
    int score = 1;
    while (!is_closed_tile_open) {
        int row = rand() % game->board->row_count;
        int column = rand() % game->board->column_count;
        if (!game->board->tiles[row][column]->is_mine) {
            open_tile(game, row, column, 'o');
            score += game->board->tiles[row][column]->value;
            ASSERT_EQ(score, game->player->score);
            is_closed_tile_open = true;
        }
    }

    destroy_game(game);
    PASS();
}

TEST count_score_when_game_failed() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 30);
    game->board = board;
    game->player->score = 30;
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column, 'o');
                ASSERT_EQ(15, game->player->score);
                break;
            }
        }
    }

    destroy_game(game);
    PASS();
}

SUITE (test_game) {
    RUN_TEST(open_mine);
    RUN_TEST(amount_of_marked);
    RUN_TEST(open_tile_with_bad_coordinates);
    RUN_TEST(count_score_after_move);
    RUN_TEST(count_score_when_game_failed);
    RUN_TEST(first_move_without_mine);
    RUN_TEST(first_move_game_state_playing);
}
