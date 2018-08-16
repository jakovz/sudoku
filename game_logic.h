#ifndef sudoku_GAME_LOGIC_H
#define sudoku_GAME_LOGIC_H

#endif //sudoku_GAME_LOGIC_H


int GAME_MODE = 0; // 0 - init mode, 1 - edit mode, 2 - solve mode
int MARK_ERRORS = 0; // 0 - do not mark errors, 1 - mark errors

void exit_game();
void reset();
void autofill();
void num_solutions();
void generate(int x, int y);
void validate_board();
void hint(int x, int y);
void save_board(char *path[]);
void redo();
void undo();
void set_cell(int x, int y, int z);
void mark_errors(int X);
void edit(char *path[]);
void solve(char *path[]);