CC = gcc
OBJS = main.o helper_functions.o exhaustive_backtracking_solver.o game_logic.o ilp_solver.o console_user_interface.o game_board_operations.o game_moves_list.o
EXEC = sudoku-console
COMP_FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
CLIB     = -L/usr/local/lib/gurobi563/lib -lgurobi56
INC      =  /usr/local/lib/gurobi563/include/

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(CLIB) -o $@ -lm
	
main.o: main.c console_user_interface.h
	$(CC) $(COMP_FLAGS) -c $*.c
helper_functions.o: helper_functions.c helper_functions.h
	$(CC) $(COMP_FLAGS) -c $*.c
exhaustive_backtracking_solver.o: exhaustive_backtracking_solver.c exhaustive_backtracking_solver.h helper_functions.h game_board_operations.h
	$(CC) $(COMP_FLAGS) -c $*.c
game_logic.o: game_logic.c game_logic.h ilp_solver.h exhaustive_backtracking_solver.h game_board_operations.h game_moves_list.h
	$(CC) $(COMP_FLAGS) -c $*.c
ilp_solver.o: ilp_solver.c ilp_solver.h game_logic.h
	$(CC) $(COMP_FLAGS) -c $*.c -I$(INC) $(CLIB) -lm
console_user_interface.o: console_user_interface.c console_user_interface.h game_logic.h helper_functions.h game_board_operations.h
	$(CC) $(COMP_FLAGS) -c $*.c
game_board_operations.o: game_board_operations.c game_board_operations.h game_moves_list.h
	$(CC) $(COMP_FLAGS) -c $*.c
game_moves_list.o: game_moves_list.c game_moves_list.h
	$(CC) $(COMP_FLAGS) -c $*.c
clean:
	rm -f *.o $(EXEC)
