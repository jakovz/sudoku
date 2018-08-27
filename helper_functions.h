#ifndef SUDOKU_HELPER_FUNCTIONS_H
#define SUDOKU_HELPER_FUNCTIONS_H

#endif /* SUDOKU_HELPER_FUNCTIONS_H */

char **str_split(char *a_str, char a_delim, size_t *count);
void get_available_numbers_for_set(int *available_numbers, int rows_index, int columns_index,
                                   int **new_solved_board, int rows_colunms_num, int rows_per_block,
                                   int columns_per_block);
int number_does_not_exist_in_block(int **game_board, int number, int row, int column, int rows_per_block, int columns_per_block);
int number_does_not_exist_in_column(int **game_board, int number, int row, int column, int rows_columns_num);
int number_does_not_exist_in_row(int **game_board, int number, int row, int column, int rows_columns_num);
int number_is_available(int **game_board, int number, int row, int column, int rows_per_block, int columns_per_block);
void print_board(int **board, int row_per_block, int columns_per_block, int game_mode, int mark_errors);
