#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game_logic.h"
#include "ilp_solver.h"
#include "exhaustive_backtracking_solver.h"
#include "game_moves_list.h"
#include "game_board_operations.h"

void solve(char *path) {
    /* loads the board from file */
    FILE *fp;
    GAME_MODE = 2;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: File doesn't exist or cannot be opened\n");
        return;
    } else {
        read_board_from_file(fp);
    }
    fclose(fp);
    EMPTY_CELLS_NUM = count_empty_cells();
    print_board();
}

void edit(char *path) {
    FILE *fp;
    GAME_MODE = 1; /*Edit mode*/
    if (path == NULL) {
        /* create an empty 9x9 board */
        ROWS_PER_BLOCK = 3;
        COLUMNS_PER_BLOCK = 3;
        init_game();
    } else {
        /* loads the board from a file */
        fp = fopen(path, "r");
        if (fp == NULL) {
            printf("Error: File cannot be opened\n");
            return;
        } else {
            read_board_from_file(fp);
        }
        fclose(fp);
    }
    EMPTY_CELLS_NUM = count_empty_cells();
    print_board();
}

void mark_errors(int X) {
    MARK_ERRORS = X;
}

void set_cell(int x, int y, int z) {
    int old;
    if (x < 0 || y < 0 || x >= ROWS_COLUMNS_NUM || y >= ROWS_COLUMNS_NUM || z < 0 || z > ROWS_COLUMNS_NUM) {
        printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
        return;
    }
    if (fixed_numbers_board[x][y] == 1 && GAME_MODE != 1) {
        printf("Error: cell is fixed\n");
        return;
    }
    old = game_board[x][y];
    game_board[x][y] = z;
    if (z != 0) {
        EMPTY_CELLS_NUM--;
    } else {
        EMPTY_CELLS_NUM++;
    }
    update_moves_list(x, y, z, old, 0);
    if (MARK_ERRORS) {
        if (check_if_board_erroneous()) {
            erroneous_board[x][y] = 1;
        }
    }
    print_board();
    if (EMPTY_CELLS_NUM == 0 && GAME_MODE == 2) {
        if (!validate_solution(0)) {
            printf("Puzzle solution erroneous\n");
            return;
        } else {
            printf("Puzzle solved successfully\n");
            GAME_MODE = 0;
        }
    }
}

void undo(int print_moves) {
    int first;
    first = 1;
    if ((*game_moves).prev == NULL) {
        printf("Error: no moves to undo\n");
        return;
    }
    while ((*game_moves).generate_autofill_command > 0 || first) {
        /* this while loop is intended to take care both of undo commands for set and of undo commands
         * for generate and autofill */
        first = 0;
        game_board[(*game_moves).x_value][(*game_moves).y_value] = (*game_moves).old_z_value;
        game_moves = (*game_moves).prev;
        print_board();
        if ((*(*game_moves).next).old_z_value == 0) {
            printf("Undo %d,%d: from %d to _\n", (*(*game_moves).next).y_value + 1, (*(*game_moves).next).x_value + 1,
                   (*(*game_moves).next).new_z_value);
            EMPTY_CELLS_NUM++;
        } else if ((*(*game_moves).next).new_z_value == 0) {
            printf("Undo %d,%d: from _ to %d\n", (*(*game_moves).next).y_value + 1, (*(*game_moves).next).x_value + 1,
                   (*(*game_moves).next).old_z_value);
            EMPTY_CELLS_NUM--;
        } else {
            if (print_moves) {
                printf("Undo %d,%d: from %d to %d\n", (*(*game_moves).next).y_value + 1,
                       (*(*game_moves).next).x_value + 1,
                       (*(*game_moves).next).new_z_value, (*(*game_moves).next).old_z_value);
            }
        }
        if ((*game_moves).generate_autofill_command == 2) {
            /* we got to the sentinel */
            game_moves = (*game_moves).prev;
            break;
        }
    }
}

void redo() {
    int new_value;
    if ((*game_moves).next == NULL) {
        printf("Error: no moves to redo\n");
        return;
    }
    /* TODO: change generate and autofill */
    new_value = (*(*game_moves).next).new_z_value;
    game_board[(*(*game_moves).next).x_value][(*(*game_moves).next).y_value] = new_value;
    game_moves = (*game_moves).next;
    print_board();
    if ((*game_moves).new_z_value == 0) {
        printf("Redo %d,%d: from %d to _\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).old_z_value);
        EMPTY_CELLS_NUM++;
    } else if ((*game_moves).old_z_value == 0) {
        printf("Redo %d,%d: from _ to %d\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).new_z_value);
        EMPTY_CELLS_NUM--;
    } else {
        printf("Redo %d,%d: from %d to %d\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).new_z_value,
               (*game_moves).old_z_value);
    }
}

void save_board(char *path) {
    int i;
    int j;
    FILE *fp;
    /*save is only available in Edit and Solve modes*/
    if ((GAME_MODE != 1) && (GAME_MODE != 2)) {
        printf("ERROR: invalid command\n");
        return;
    } else if (GAME_MODE == 1) {
        /*Edit mode*/
        if (check_if_board_erroneous()) {
            printf("Error: board contains erroneous values\n");
            return;
        }
        if (validate_solution(0) == 0) {
            printf("Error: board validation failed\n");
            return;
        }
    }
    fp = fopen(path, "w+");
    if (fp == NULL) {
        printf("Error: File cannot be created or modified\n");
        return;
    } else {
        if (GAME_MODE == 1) {
            /*if in Edit mode: clear fixed_numbers_board (save_board_to_file marks all filled cells as fixed when in Edit mode)*/
            for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
                for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                    fixed_numbers_board[i][j] = 0;
                }
            }
        }
        /*save game_board to file*/
        save_board_to_file(fp);
    }
    fclose(fp);
    printf("Saved to: %s\n", path);
}

void get_hint(int x, int y) {
    int tmp;
    if (x > ROWS_COLUMNS_NUM || y > ROWS_COLUMNS_NUM) {
        printf("Error: value not in range 1-%d", ROWS_COLUMNS_NUM);
        return;
    }
    tmp = x;
    x = y - 1;
    y = tmp - 1;
    if (check_if_board_erroneous()) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    if (fixed_numbers_board[x][y] == 1) {
        printf("Error: cell is fixed\n");
        return;
    }
    if (game_board[x][y] != 0) {
        printf("Error: cell already contains a value\n");
        return;
    }
    if (solve_board(game_board, ROWS_COLUMNS_NUM, ROWS_PER_BLOCK, COLUMNS_PER_BLOCK, 1, solved_board)) {
        printf("Hint: set cell to %d\n", solved_board[x][y]);
    } else {
        printf("Error: board is unsolvable\n");
        return;
    }
}

int validate_solution(int print_messages) {
    if (check_if_board_erroneous()) {
        if (print_messages) {
            printf("Error: board contains erroneous values\n");
        }
        return 0;
    } else if (solve_board(game_board, ROWS_COLUMNS_NUM, ROWS_PER_BLOCK, COLUMNS_PER_BLOCK, 1, solved_board)) {
        if (print_messages) {
            printf("Validation passed: board is solvable\n");
        }
        return 1;
    } else {
        if (print_messages) {
            printf("Validation failed: board is unsolvable\n");
        }
        return 0;
    }
}

int try_generate(int x) {
    int x_index;
    int y_index;
    int i;
    int j;
    int count;
    int *legal_values;
    int rand_value;
    legal_values = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    for (i = 0; i < x; i++) {
        count = 0;
        x_index = rand() % ROWS_COLUMNS_NUM;
        y_index = rand() % ROWS_COLUMNS_NUM;
        get_available_numbers_for_set(legal_values, x_index, y_index);
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (legal_values[j] == 0) {
                count++;
            }
        }
        if (count == 0) {
            /* no legal values available for this cell */
            clear_game_boards();
            free(legal_values);
            return 0;
        }
        rand_value = rand() % count; /* randomizing a legal value */
        j = 0;
        while (rand_value != 0) {
            /* getting the actual value out of the legal_values array */
            if (legal_values[j] == 0) {
                rand_value--;
            }
            j++;
        }
        game_board[x_index][y_index] = legal_values[j-1];
    }
    free(legal_values);
    if (!solve_board(game_board, ROWS_COLUMNS_NUM, ROWS_PER_BLOCK, COLUMNS_PER_BLOCK, 1, solved_board)) {
        return 0;
    }
    return 1;
}

void generate(int x, int y) {
    int i;
    int x_index;
    int y_index;
    int **tmp_board;
    tmp_board = (int **) malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        tmp_board[i] = (int *) malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    }
    if (EMPTY_CELLS_NUM < ROWS_COLUMNS_NUM * ROWS_COLUMNS_NUM) {
        printf("Error: board is not empty\n");
        return;
    }
    if (x > EMPTY_CELLS_NUM || y > EMPTY_CELLS_NUM) {
        printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
        return;
    }
    i = 0;
    while (!try_generate(x) && i < 1000) {
        i++;
    }
    if (i >= 1000) {
        printf("Error: puzzle generator failed\n");
        return;
    }
    /* if the program got here, it means that the board was generated successfully */
    copy_board(solved_board, tmp_board, 0);
    for (i = 0; i < y; i++) {
        x_index = rand() % ROWS_COLUMNS_NUM;
        y_index = rand() % ROWS_COLUMNS_NUM;
        tmp_board[x_index][y_index] = 0;
    }
    update_moves_list(0, 0, 0, 0, 2); /* inserting sentinel to indicate this is a start of generate function */
    copy_board(tmp_board, game_board, 1);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        free(tmp_board[i]);
    }
    free(tmp_board);
    print_board();
}

void num_solutions() {
    int i;
    int j;
    int ans;
    int **exhaustive_board;
    exhaustive_board = (int **) malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    current_indicators_board = (int **) malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    if (current_indicators_board == NULL || exhaustive_board == NULL) {
        printf("Error: num_solutions failed\n");
        exit(-1);
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        current_indicators_board[i] = (int *) malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        exhaustive_board[i] = (int *) malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        if (current_indicators_board[i] == NULL || exhaustive_board[i] == NULL) {
            printf("Error: num_solutions failed\n");
            exit(-1);
        }
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            /*current_game_board[i][j] = game_board[i][j];*/
            if (game_board[i][j] > 0) {
                current_indicators_board[i][j] = 1;
            } else {
                current_indicators_board[i][j] = 0;
            }
        }
    }
    if (check_if_board_erroneous() == 1) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    copy_board(game_board, exhaustive_board, 0);
    ans = exhaustive_backtracking(0, 0, exhaustive_board, 0);
    printf("Number of solutions: %d\n", ans);
    if (ans == 1) {
        printf("This is a good board!\n");
    } else if (ans > 1) {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        free(current_indicators_board[i]);
        free(exhaustive_board[i]);
    }
    free(current_indicators_board);
    free(exhaustive_board);
}

int get_autofill_value(int x, int y) {
    int i;
    int available_number;
    int count; /*  counts the number of available numbers that can be filled in the empty cell. */
    count = 0;
    available_number = 0;
    for (i = 1; i <= ROWS_COLUMNS_NUM; i++) {
        /* iterating all possible numbers */
        if (number_is_available(i, x, y)) {
            count++;
            available_number = i;
        }
    }
    if (count > 1 || count == 0) {
        return 0;
    } else {
        return available_number;
    }
}

void autofill() {
    int i;
    int j;
    int new_value;
    int **filled_board;
    if (check_if_board_erroneous()) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    filled_board = malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    if (filled_board == NULL) {
        printf("Error: autofill failed\n");
        exit(-1);
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        filled_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        if (filled_board[i] == NULL) {
            printf("Error: autofill failed\n");
            exit(-1);
        }
    }
    copy_board(game_board, filled_board, 0);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (game_board[i][j] == 0) {
                /* we are going to check if a cell has an obvious value only if the cell is empty */
                if ((new_value = get_autofill_value(i, j)) > 0) {
                    /* means that there is an autofill option */
                    filled_board[i][j] = new_value; /* we do not want the new values to affect autofill. */
                    printf("Cell <%d,%d> set to %d\n", i, j, new_value);
                }
            }
        }
    }
    update_moves_list(0, 0, 0, 0, 2); /* inserting sentinel to indicate this is a start of generate function */
    copy_board(filled_board, game_board, 1);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        free(filled_board[i]);
    }
    print_board();
    free(filled_board);
}

void restart_game() {
    while ((*game_moves).prev != NULL) {
        undo(0);
    }
    clear_moves_list();
    printf("Board reset\n");
}

void exit_game() {
    printf("Exiting...\n");
    if (GAME_ALREADY_INITIALIZED) {
        clear_moves_list();
        free_game_boards();
    }
}
