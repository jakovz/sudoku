#ifndef sudoku_GAME_LOGIC_H
#define sudoku_GAME_LOGIC_H
#endif //sudoku_GAME_LOGIC_H

struct game_move{
    struct game_move *next;
    struct game_move *prev;
    int x_value;
    int y_value;
    int new_z_value;
    int old_z_value;
} *game_moves;

int **game_board;
int **fixed_numbers_board;
int **erroneous_board;
int ROWS_PER_BLOCK;
int COLUMNS_PER_BLOCK;
int ROWS_COLUMNS_NUM;
int EMPTY_CELLS_NUM;
int GAME_MODE; // 0 - init mode, 1 - edit mode, 2 - solve mode
int MARK_ERRORS; // 0 - do not mark errors, 1 - mark errors

void execute_solve(char **params);
void execute_edit(char **params);
void execute_set_cell(char **params);
void execute_generate(char **params);
void execute_save_board(char **params);
void execute_get_hint(char **params);
void execute_mark_errors(char **params);
void exit_game();
void restart_game();
void get_available_numbers_for_set(int available_numbers[ROWS_COLUMNS_NUM], int rows_index, int columns_index,
                                   int new_solved_board[ROWS_COLUMNS_NUM][ROWS_COLUMNS_NUM])
void autofill();
void num_solutions();
void generate(int x, int y);
int validate_solution();
void get_hint(int x, int y);
void save_board(char *path);
void redo();
void undo();
void set_cell(int x, int y, int z);
void mark_errors(int X);
void edit(char *path);
void solve(char *path);
void init_game();