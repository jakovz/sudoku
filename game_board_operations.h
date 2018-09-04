#ifndef SUDOKU_GAME_BOARD_OPERATIONS_H
#define SUDOKU_GAME_BOARD_OPERATIONS_H

#endif /* SUDOKU_GAME_BOARD_OPERATIONS_H */

int **game_board;
int **fixed_numbers_board;
int **erroneous_board;
int **solved_board;
int ROWS_PER_BLOCK;
int COLUMNS_PER_BLOCK;
int ROWS_COLUMNS_NUM;
int EMPTY_CELLS_NUM;
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
int number_does_not_exist_in_row(int number, int row, int column, int mark_as_erroneous);

/* checks if the given number exists in the column
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_does_not_exist_in_column(int number, int row, int column, int mark_as_erroneous);

/* checks if the given number exists in the block
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_does_not_exist_in_block(int number, int row, int column, int mark_as_erroneous);

/* checks if the given number exists in the row
 * input:
 * int number - the number to check for
 * int row - the row of the number
 * int column - the column of the number
 * int mark_as_erroneous - whether to mark erroneous values (1) or not (2)
 * output: 1 if the number is available and 0 otherwise
 * */
int number_is_available(int number, int row, int column, int mark_as_erroneous);

/* checks if the game board contains any erroneous values
 * input: none
 * output: 1 if the board contains erroneous values and 0 otherwise
 * */
int check_if_board_erroneous();

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