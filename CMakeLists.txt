cmake_minimum_required(VERSION 3.9)
project(sudoku C)

set(CMAKE_C_STANDARD 90)

add_executable(sudoku main.c console_user_interface.c game_logic.c ilp_solver.c puzzle_generator.c exhaustive_backtracking_solver.c exhaustive_backtracking_solver.h game_logic.h ilp_solver.h puzzle_generator.h console_user_interface.h helper_functions.c helper_functions.h gurobi_c.h game_moves_list.c game_moves_list.h game_board_operations.c game_board_operations.h)