#ifndef sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H
#define sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H

#endif /* sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H */

int **current_indicators_board;
struct StackNode {
    int rows_index;
    int columns_index;
    int current;
    struct StackNode *next;
};

struct StackNode *newNode(int rows_index, int columns_index, int current);

int isEmpty(struct StackNode *root);

/* pushes the StackNode to the stack
 * StackNode ** root - the root of the stack
 * int rows_index - the index of the row
 * int columns_index - the index of the columns
 * int current - the current value of the cell
 * */
void push(struct StackNode **root, int rows_index, int columns_index, int current);

/* pops the root of that stack
 * input:
 * StackNode ** root - the root of the stack
 * output: StackNode containing the values of old root
 * */
struct StackNode pop(struct StackNode **root);

/* calculates the number of the solutions exist for a given board using exhaustive backtracking
 * input:
 * int rows_index - the index of the rows to start from - should be called with 0
 * int columns_index - the index of the columns to start from - should be called with 0
 * int **game_board - the given game board
 * int current_value - the current value of the cell we are working on - should be called with 0
 * output:
 * the number of possible solutions the board has
 * */
int exhaustive_backtracking(int rows_index, int columns_index, int **game_board, int cell_current_value);