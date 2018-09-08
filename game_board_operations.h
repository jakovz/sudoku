#ifndef SUDOKU_GAME_BOARD_OPERATIONS_H
#define SUDOKU_GAME_BOARD_OPERATIONS_H

#endif /* SUDOKU_GAME_BOARD_OPERATIONS_H */

/* helper functions which are related to the game board */


int **game_board; /* the game board */
int **fixed_numbers_board; /* a board which holds the indicators (1 or 0) for the fixed values in the game board */
int **erroneous_board; /* a board which holds the indicators (1 or 0) for the erroneous values in the game board */
int **solved_board; /* a board which holds the solution of the game board */
int ROWS_PER_BLOCK; /* the number of rows a block in the game board has */
int COLUMNS_PER_BLOCK; /* the number of columns a block in the game board has */
int ROWS_COLUMNS_NUM; /* the number of rows(=columns) the game board has */
int EMPTY_CELLS_NUM; /* the number of empty cell the game board has in every moment of the game */
int GAME_MODE; /* 0 - init mode, 1 - edit mode, 2 - solve mode */
int MARK_ERRORS; /* 0 - do not mark errors, 1 - mark errors */
int GAME_ALREADY_INITIALIZED; /* 0 - not initialized yet, 1 - already initialized */

/* renders the game board out of a given file
 * input:
 * FILE *file_descriptor - the file descriptor of the file from which the board will be rendered
 * output: none
 * */
void read_board_from_file(FILE *file_descriptor);

/* saves the game board to a given file
 * input:
 * FILE *file_descriptor - the file descriptor of the file to which the board will be saved
 * output: none
 * */
void save_board_to_file(FILE *file_descriptor);

/* copies a whole board to another board
 * input:
 * int **from - the board to copy from
 * int **to - the board to copy to
 * int save_moves - 1 to save the coping moves and 0 otherwise
 * output: none
 * */
void copy_board(int **from, int **to, int save_moves);

/* clears all game boards related to the game
 * input: none
 * output: none
 * */
void clear_game_boards();

/* initializes all game structures, boards and parameters
 * input: none
 * output: none
 */
void init_game();

/* checks if the given number exists in the row
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * */
int number_does_not_exist_in_row(int number, int row, int column, int mark_as_erroneous, int save_moves);

/* checks if the given number exists in the column
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_does_not_exist_in_column(int number, int row, int column, int mark_as_erroneous, int save_moves);

/* checks if the given number exists in the block
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_does_not_exist_in_block(int number, int row, int column, int mark_as_erroneous, int save_moves);

/* checks if the given number exists in the row
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_is_available(int number, int row, int column, int mark_as_erroneous, int save_moves);

/* checks if the game board contains any erroneous values
 * input: none
 * output: 1 if the board contains erroneous values and 0 otherwise
 * */
int check_if_board_erroneous();

/* checks if the specific cell in game board contains an erroneous value
 * input: none
 * output: 1 if the cell is erroneous and 0 otherwise
 * */
int check_if_value_erroneous(int x, int y, int set_erroneous_moves, int save_erroneous_moves);

/* frees the memory consumed by all game boards
 * input: none
 * output: none
 * */
void free_game_boards();

/* gets an array of size ROWS_COLUMNS_NUM number and returns the number of occurrences of the number in the row/column/block
 * for each (index-1) in the array
 * input:
 * int *available_numbers - list of size ROWS_COLUMNS_NUM
 * int rows_index - the index of the cell to check for
 * int columns_index - the index of the columns ot check for
 */
void get_available_numbers_for_set(int *available_numbers, int rows_index, int columns_index);

/* prints the game board in the format specified in the instructions.
 * input: none
 * output: none
 */
void print_board();

/* count the number of empty cells in the game board
 * input: none
 * output: the number of empty cells in the game board
 * */
int count_empty_cells();

/* check if there are erroneous cells  which are no longer erroneous after committing a move and updates the board
 * and the moves list
 * input:
 * int old - the old value of the cell
 * int x  - the x value of the cell
 * int y - the y value of the cell
 * output: none
 * */
void cancel_erroneous_values(int old, int x, int y);

/* gets an array of size ROWS_COLUMNS_NUM number and returns the number of occurrences of the number in the row/column/block
 * for each (index-1) in the array. unlike get_available_number_for_set, it does not take care of the game_moves list
 * and gets the game board it should check as an argument.
 * input:
 * int *available_numbers - list of size ROWS_COLUMNS_NUM
 * int rows_index - the index of the cell to check for
 * int columns_index - the index of the columns ot check for
 * int **tmp_game_board - the board we should check for available numbers at
 */
void
get_available_numbers_for_backtrack(int **tmp_game_board, int *available_numbers, int rows_index, int columns_index);