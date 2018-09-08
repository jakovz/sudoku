#ifndef SUDOKU_GAME_MOVES_LIST_H
#define SUDOKU_GAME_MOVES_LIST_H

#endif /* SUDOKU_GAME_MOVES_LIST_H */

struct game_move {
    struct game_move *next;
    struct game_move *prev;
    int x_value;
    int y_value;
    int new_z_value;
    int old_z_value;
    int generate_autofill_command; /* 1 for a transition command, 2 for generate/autofill start,
                                    * 3 for generate/autofill end, 4 for set start, 5 for set end*/
    int old_value_erroneous; /* 0 for non-erroneous, 1 for erroneous */
    int new_value_erroneous; /* 0 for non-erroneous, 1 for erroneous */
} *game_moves;

/* initializes the sentinel indicates we are on the first move in the list.
 * should be called before any command using the game_moves list when this list is empty.
 * input: none
 * output: none
 * */
void initialize_game_moves_list();

/* updates the game_moves list with the new move given as input and sets the new move to be the current
 * input:
 * int x - the x value of the move
 * int y  - the y value of the move
 * int z - the value of the cell after the move
 * int old - the value of the cell before the move
 * int autofill_generate_value - the type of the command (see autofill_generate_command of struct game_move)
 * int old_value_erroneous - 1 if the old value was erroneous and 0 otherwise
 * int old_value_erroneous - 1 if the new value is erroneous and 0 otherwise
 * output: none
 * */
void update_moves_list(int x, int y, int z, int old, int autofill_generate_value, int old_value_erroneous,
                       int new_value_erroneous);

/* clear all the moves from the list and frees the corresponding memory
 * input: none
 * output: none
 * */
void clear_moves_list();

/* clear all the move which come after the current move in the game_moves list and frees the corresponding memory
 * input: none
 * output: none*/
void free_next_moves();