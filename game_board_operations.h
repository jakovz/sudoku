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

void read_board_from_file(FILE *file_descriptor);
void save_board_to_file(FILE *file_descriptor);
void copy_board(int **from, int **to, int save_moves);
void clear_game_boards();
void init_game();
int number_does_not_exist_in_row(int number, int row, int column);
int number_does_not_exist_in_column(int number, int row, int column);
int number_does_not_exist_in_block(int number, int row, int column);
int number_is_available(int number, int row, int column);
int check_if_board_erroneous();
void free_game_boards();
void get_available_numbers_for_set(int *available_numbers, int rows_index, int columns_index);
void print_board();