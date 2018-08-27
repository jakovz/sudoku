#ifndef sudoku_GAME_LOGIC_H
#define sudoku_GAME_LOGIC_H
#endif /* sudoku_GAME_LOGIC_H */

void execute_set_cell(char **params);
void execute_generate(char **params);
void execute_save_board(char **params);
void execute_get_hint(char **params);
void execute_mark_errors(char **params);
void exit_game();
void restart_game();
void autofill();
void num_solutions();
void generate(int x, int y);
int validate_solution();
void get_hint(int x, int y);
void save_board(char *path);
void redo();
void undo(int print_moves);
void set_cell(int x, int y, int z);
void mark_errors(int X);
void edit(char *path);
void solve(char *path);
void init_game();