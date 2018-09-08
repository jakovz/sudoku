#ifndef sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H
#define sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H

#endif /* sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H */

/* the functions who are responsible for giving the number of the solution exist for the
 * game board through exhaustive_backtracking. */

int **current_indicators_board;
struct StackNode {
    int rows_index;
    int columns_index;
    int current;
    struct StackNode *next;
};

/* calculates the number of the solutions exist for a given board using exhaustive backtracking
 * input:
 * int rows_index - the index of the rows to start from - should be called with 0
 * int columns_index - the index of the columns to start from - should be called with 0
 * int **game_board - the given game board
 * int current_value - the current value of the cell we are working on - should be called with 0
 * output:
 * the number of possible solutions the board has
 * */
int exhaustive_backtracking(int rows_index, int columns_index, int **tmp_game_board, int cell_current_value);

