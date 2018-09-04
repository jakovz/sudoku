#ifndef sudoku_GAME_LOGIC_H
#define sudoku_GAME_LOGIC_H
#endif /* sudoku_GAME_LOGIC_H */

/* frees all the game structures and exists the game */
void exit_game();

/* undoing all moves since last board initialization */
void restart_game();

/* fills all obvious values - as it was declared in the project guide - with their value */
void autofill();

/* returns the number of possible solutions exist for the given board */
void num_solutions();


/* if the current board is empty - generates a new game board
 * input:
 * x - number of cells which will be randomly chosen and filled by the method
 * y - number of game board cells that will be left filled, after a full generation of a board was completed.
 * output: prints the generated game board
 * */
void generate(int x, int y);

/* validates the correctness of the current game board solution
 * input:
 * int print_board - indicates whether method console prints should be displayed
 * output: returns 1 if the game board is solvable and 0 otherwise
 */
int validate_solution(int print_board);

/* prints a hint for the given cell
 * input:
 * int x - the x value of the cell
 * int y - the y value of the cell
 * output: prints the possible value for the cell
 */
void get_hint(int x, int y);

/* saves the game board to the given path
 * input:
 * char * path - the path in which the file will be saved
 * output: none
 */
void save_board(char *path);

/* redo the last undone game move committed
 * input:
 * int print_moves - indicates whether method's console prints should be displayed
 * output: none
 */
void redo(int print_moves);

/* undo the last game move committed
 * input:
 * int print_moves - indicates whether method's console prints should be displayed
 * output: none
 */
void undo(int print_moves);

/* sets the given cell in the game board to be the given value
 * input:
 * int x - the x coordinated of the board
 * int y - the y coordinated of the board
 * int z - the value to be set
 * output: none
 */
void set_cell(int x, int y, int z);

/* sets mark_errors to 1 or 0 according to the given value
 * input:
 * int X - the value to be set for mark_errors
 * output - none
 */
void mark_errors(int X);

/* changes game mode to edit mode and loads the board. the board is set to be an empty board of size 9x9 if a path wasn't
 * specified and the board as it should be rendered (by following the guidelines) from a given file otherwise.
 * input:
 * char * path - the path from which the board will be loaded (optional)
 * output:
 * none
 */
void edit(char *path);

/* changes game mode to solve mode and loads the board. the board is set to be the board as it should be rendered
 * (by following the guidelines) from a given file.
 * input:
 * char * path - the path from which the board will be loaded
 * output:
 * none
 */
void solve(char *path);