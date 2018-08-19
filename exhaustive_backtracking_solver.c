#include "exhaustive_backtracking_solver.h"
#include "game_logic.h"

int exhaustive_backtracking(int rows_index, int columns_index, int new_solved_board[ROWS_COLUMNS_NUM][ROWS_COLUMNS_NUM], int current) {
    /*returns num of possible solutions to a given board (if the current board is solvable) using an exhaustive (deterministic) backtracking algorithm*/
    int available_numbers[ROWS_COLUMNS_NUM] = {0};
    int num_of_solutions;
    int counter;
    int i;
    int j;
    int p;
    counter = 0;
    num_of_solutions = 0;

    while (new_solved_board[rows_index][columns_index] > 0) {
        if ((columns_index + 1) < ROWS_COLUMNS_NUM) {
            columns_index++;
        } else if ((rows_index + 1) < ROWS_COLUMNS_NUM) {
            columns_index = 0;
            rows_index++;
        }
            /* if this is the last cell and its value is already set*/
        else if (((rows_index + 1) == ROWS_COLUMNS_NUM) && ((columns_index + 1) == ROWS_COLUMNS_NUM)) {
            //return 0; //TODO: doesn't finish when the last cell is filled
            num_of_solutions++;


            p = new_solved_board[rows_index][columns_index];
            new_solved_board[rows_index][columns_index] = p+1;
        }
    }

    get_available_numbers_for_set(available_numbers, rows_index, columns_index, new_solved_board);
    /* creating the actual available numbers array*/
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (available_numbers[i] == 0 && (i + 1) > current) {
            counter++;
        }
    }
    /* backtracking from the first cell - means that the board is unsolvable, and there is no valid solution as is.*/
    if (counter == 0 && rows_index == 0 && columns_index == 0) {
        return -1;
    }
    if (counter == 0) {
        return 1;
    }
    for (p = 0; p < ROWS_COLUMNS_NUM; p++) {
        if (available_numbers[p] == 0 && (p + 1) > current) {
            new_solved_board[rows_index][columns_index] = p + 1;
            break;
        }
    }
    /* call the function with the next indexes.*/
    if ((columns_index + 1) < ROWS_COLUMNS_NUM) {
        if (exhaustive_backtracking(rows_index, columns_index + 1, new_solved_board, 0) == 1) { //TODO: stackkk instead of recursion
            for (i = rows_index; i < ROWS_COLUMNS_NUM; i++) {
                if(i == rows_index){
                    for (j = columns_index; j < ROWS_COLUMNS_NUM; j++) {
                        if (current_indicators_board[i][j] == 0) {
                            new_solved_board[i][j] = 0;
                        }
                    }
                }
                else{
                    for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                        if (current_indicators_board[i][j] == 0) {
                            new_solved_board[i][j] = 0;
                        }
                    }
                }
            }
            current = p + 1;
            return exhaustive_backtracking(rows_index, columns_index, new_solved_board, current); //TODO: stackkk instead of recursion
        }
    } else if ((rows_index + 1) < ROWS_COLUMNS_NUM) {
        if (exhaustive_backtracking(rows_index + 1, 0, new_solved_board, 0) == 1) { //TODO: stackkk instead of recursion
            for (i = rows_index; i < ROWS_COLUMNS_NUM; i++) {
                if(i == rows_index){
                    for (j = columns_index; j < ROWS_COLUMNS_NUM; j++) {
                        if (current_indicators_board[i][j] == 0) {
                            new_solved_board[i][j] = 0;
                        }
                    }
                }
                else{
                    for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                        if (current_indicators_board[i][j] == 0) {
                            new_solved_board[i][j] = 0;
                        }
                    }
                }
            }
            current = p + 1;
            return exhaustive_backtracking(rows_index, columns_index, new_solved_board, current); //TODO: stackkk instead of recursion
        }
    }
    /*the last cell was successfully assigned the value p+1*/
    num_of_solutions++;
    //return 0; //TODO: doesn't finish when the last cell is filled
}
