#ifndef SUDOKU_GAME_MOVES_LIST_H
#define SUDOKU_GAME_MOVES_LIST_H

#endif /* SUDOKU_GAME_MOVES_LIST_H */

struct game_move{
    struct game_move *next;
    struct game_move *prev;
    int x_value;
    int y_value;
    int new_z_value;
    int old_z_value;
    int generate_autofill_command; /* 0 for non-generate, 1 for generate/autofill, 2 for generate/autofill start */
} *game_moves;


void initialize_game_moves_list();
void update_moves_list(int x, int y, int z, int old, int autofill_generate_value);
void clear_moves_list();
void free_next_moves();