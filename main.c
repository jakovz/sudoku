#include <malloc.h>
#include <time.h>
#include "console_user_interface.h"

/* The way we chose to split our code:
 * game_logic.c - all the functions commit the commands themselves
 * game_board_operations.c - helper functions which are related to the game board
 * game_moves.c - all the functions which related to the game moves list (the "behind the scenes" of undo and redo commands)
 * console_user_interface.c - all the functions which are related to interaction with the user (i.e parsing commands, game play etc.)
 * ilp_solver.c - the functions who are responsible for solving the game board through Gurobi (ILP).
 * exhaustive_backtracking_solver.c - the functions who are responsible for giving the number of the solution exist for the
 * game board through exhaustive_backtracking.
 * helper_functions - generic helper functions which are not directly related to the game itself.
 *
 * In this way of code splitting, each code part is responsible for one specific and dedicated purpose.
 * it is really easy to understand where each function will be found and moreover, the number of functions and size of
 * each code file is pretty much balanced that way.
 *
 * This is the way which gave us the optimal value for both explicit declarations and splitting, and balancing the code
 * size between the files.
 *
 * */

int main() {
    play_game();
    return 0;
}