#include "greatest.h"
#include "../hof.h"

TEST load_file_number_of_players()
{
    char hof_file[] = "tests/score/optimal_size";
    Player list_of_players[PLAYERS_LIMIT];

    ASSERT_EQ(5, read_scores(hof_file, list_of_players));
    PASS();
}

TEST load_file_over_limit()
{
    char hof_file[] = "tests/score/over_size";
    int high_size_of_list = PLAYERS_LIMIT + 5;
    Player list_of_players[high_size_of_list];

    ASSERT_EQ(PLAYERS_LIMIT, read_scores(hof_file, list_of_players));
    PASS();
}

TEST load_nonexistent_file()
{
    char hof_file[] = "tests/score/nonexistent";
    Player list_of_players[PLAYERS_LIMIT];
    ASSERT_EQ(0, read_scores(hof_file, list_of_players));
    PASS();
}

TEST compare_loaded_and_saved_players()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"jozo", 55},
        {"alena", 43},
    };
    int size_of_input_list = 3;
    char hof_file[] = "tests/score/for_saving";

    save_scores(hof_file, list_of_players, size_of_input_list);
    Player test_list_of_players[size_of_input_list];
    int size_of_test_list =
        read_scores(hof_file, test_list_of_players);

    ASSERT_EQ(size_of_input_list, size_of_test_list);
    for (int i = 0; i < size_of_test_list; i++)
    {
        ASSERT_EQ(list_of_players[i].score, test_list_of_players[i].score);
        ASSERT_STR_EQ(list_of_players[i].name, test_list_of_players[i].name);
    }
    remove(hof_file);
    PASS();
}

TEST save_player_over_limit()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"jozo", 55},
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        {"kiko", 4},
        {"lubo", 2}};
    int size = PLAYERS_LIMIT + 1;
    char hof_file[] = "tests/score/for_saving";
    Player *player = (Player *)calloc(1, sizeof(Player));
    strcpy(player->name, "lucia");
    player->score = 26;

    ASSERT_FALSE(add_player_to_list(list_of_players, &size, *player));
    free(player);
    remove(hof_file);
    PASS();
}

TEST add_player_to_optimal_list()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"alex", 18},
        {"lubo", 2}};
    int size = 3;
    Player *player = (Player *)calloc(1, sizeof(Player));
    strcpy(player->name, "lucia");
    player->score = 26;

    int size_before_adding = size;
    add_player_to_list(list_of_players, &size, *player);
    ASSERT_EQ(size_before_adding + 1, size);
    free(player);
    PASS();
}

TEST sort_full_list_of_players_correctly()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 29},
        {"jozo", 55},
        {"alena", 65},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 2},
        {"kiko", 4},
        {"lubo", 5}};
    int size = 10;

    sort_descending(list_of_players, size);
    ASSERT_EQ(list_of_players[0].score, 65);    
    ASSERT_EQ(list_of_players[size-1].score, 2);
    PASS();
}

TEST add_player_to_full_unsorted_list_first_place()
{
    Player first_player = {"peter", 99};
    Player last_player = {"lubo", 2};

    int size = PLAYERS_LIMIT;

    Player list_of_players[PLAYERS_LIMIT] = {
        {"jozo", 55},
        first_player,
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        last_player,
        {"kiko", 4}};

    Player new_player = {"first", 150};
    add_player_to_list(list_of_players, &size, new_player);
    ASSERT_EQ(size, PLAYERS_LIMIT);
    ASSERT_EQ(list_of_players[0].score, new_player.score);
    ASSERT_EQ(list_of_players[1].score, first_player.score);
    ASSERT_NEQ(list_of_players[size - 1].score, last_player.score);
    PASS();
}

TEST add_player_to_full_unsorted_list_middle_place()
{
    Player first_player = {"peter", 99};
    Player last_player = {"lubo", 2};

    int size = PLAYERS_LIMIT;

    Player list_of_players[PLAYERS_LIMIT] = {
        {"jozo", 55},
        first_player,
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        last_player,
        {"kiko", 4}};

    Player new_player = {"first", 70};
    add_player_to_list(list_of_players, &size, new_player);
    ASSERT_EQ(size, PLAYERS_LIMIT);
    ASSERT_EQ(list_of_players[0].score, first_player.score);
    ASSERT_EQ(list_of_players[1].score, new_player.score);
    ASSERT_NEQ(list_of_players[size - 1].score, last_player.score);
    PASS();
}

TEST add_player_to_full_unsorted_list_last_place()
{
    Player first_player = {"peter", 99};
    Player last_player = {"lubo", 2};

    int size = PLAYERS_LIMIT;

    Player list_of_players[PLAYERS_LIMIT] = {
        {"jozo", 55},
        first_player,
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        last_player,
        {"kiko", 4}};

    Player new_player = {"first", 3};
    add_player_to_list(list_of_players, &size, new_player);
    ASSERT_EQ(size, PLAYERS_LIMIT);
    ASSERT_EQ(list_of_players[0].score, first_player.score);
    ASSERT_EQ(list_of_players[size - 1].score, new_player.score);
    ASSERT_NEQ(list_of_players[size - 1].score, last_player.score);
    PASS();
}

TEST add_player_to_full_unsorted_list_last_place_with_same_score_as_last()
{
    Player first_player = {"peter", 99};
    Player last_player = {"lubo", 2};

    int size = PLAYERS_LIMIT;

    Player list_of_players[PLAYERS_LIMIT] = {
        {"jozo", 55},
        first_player,
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        last_player,
        {"kiko", 4}};

    Player new_player = {"first", 2};
    add_player_to_list(list_of_players, &size, new_player);
    ASSERT_EQ(size, PLAYERS_LIMIT);
    ASSERT_EQ(list_of_players[0].score, first_player.score);
    ASSERT_STR_EQ(list_of_players[size - 1].name, "first");
    PASS();
}

SUITE(test_hall_of_fame)
{
    RUN_TEST(load_file_number_of_players);
    RUN_TEST(load_file_over_limit);
    RUN_TEST(load_nonexistent_file);
    RUN_TEST(compare_loaded_and_saved_players);
    RUN_TEST(save_player_over_limit);
    RUN_TEST(add_player_to_optimal_list);
    RUN_TEST(sort_full_list_of_players_correctly);
    RUN_TEST(add_player_to_full_unsorted_list_first_place);
    RUN_TEST(add_player_to_full_unsorted_list_middle_place);
    RUN_TEST(add_player_to_full_unsorted_list_last_place);
    RUN_TEST(add_player_to_full_unsorted_list_last_place_with_same_score_as_last);
}
