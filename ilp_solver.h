#ifndef sudoku_ILP_SOLVER_H
#define sudoku_ILP_SOLVER_H

#endif /* sudoku_ILP_SOLVER_H */

/* solves a given sudoku board using ILP algorithms.
 * input:
 * int **game_board - a two dimensional array representing the matrix
 * int rows_columns - the number of rows/columns in game_board
 * int rows_per_block - the number of rows per block
 * int columns_per_block - the number of columns per block
 * int fill_solved_board_with solution - 1 if we want to save the solution to solved_board, 0 otherwise
 * int **solved_board - the board which will hold the solution if fill_solved_board_with solution is set to 1
 * output:
 * returns 1 if the board was solved successfully and 0 otherwise
 */
int solve_board(int **game_board, int rows_columns, int rows_per_block, int columns_per_block,
                int fill_solved_board_with_solution, int **solved_board);