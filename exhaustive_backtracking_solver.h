#ifndef sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H
#define sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H

#endif /* sudoku_EXHAUSTIVE_BACKTRACKING_SOLVER_H */

int **current_indicators_board;
int ROWS_COLUMNS_NUM;
struct StackNode {
    int rows_index;
    int columns_index;
    int current;
    struct StackNode* next;
};

struct StackNode* newNode(int rows_index, int columns_index, int current);
int isEmpty(struct StackNode *root);
void push(struct StackNode** root, int rows_index, int columns_index, int current);
struct StackNode pop(struct StackNode** root);
int exhaustive_backtracking(int rows_index, int columns_index, int **game_board, int cell_current_value, int rows_columns_num,
                            int rows_per_block, int columns_per_block);