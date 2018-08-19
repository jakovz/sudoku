#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <mem.h>
#include <time.h>
#include "game_logic.h"
#include "helper_functions.h"
#include "ilp_solver.h"
#include "exhaustive_backtracking_solver.h"


void execute_set_cell(char **params) {
    int set_cell_params[3];
    int i;
    char *next;
    for (i = 0; i < 3; i++) {
        set_cell_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    set_cell_params[0] = set_cell_params[0] - 1;
    set_cell_params[1] = set_cell_params[1] - 1;
    set_cell(set_cell_params[1], set_cell_params[0], set_cell_params[2]);
}

void execute_generate(char **params) {
    int generate_params[2];
    int i;
    char *next;
    for (i = 0; i < 2; i++) {
        generate_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
            return;
        }
    }
    generate(generate_params[0], generate_params[1]);
}

void execute_save_board(char **params) {
    save_board(params[0]);
}

void execute_get_hint(char **params) {
    int get_hint_params[2];
    int i;
    char *next;
    for (i = 0; i < 2; i++) {
        get_hint_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    get_hint(get_hint_params[0], get_hint_params[1]);
}

void execute_mark_errors(char **params) {
    int mark_errors_param;
    char *next;
    mark_errors_param = strtol(params[0], &next, 10);
    if (next <= params[0] || mark_errors_param > 1) {
        printf("Error: the value should be 0 or 1\n");
        return;
    }
}

void copy_board(int **from, int **to, int save_moves) {
    int i;
    int j;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (save_moves) {
                if (from[i][j] != 0 && from[i][j] != to[i][j]) {
                    set_cell(i, j, from[i][j]);
                }
            } else {
                to[i][j] = from[i][j];
            }
        }
    }
}

void solve(char *path) {
    // loads the board from file
    FILE *fp;
    int i;
    int j;
    int c;
    GAME_MODE = 2;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: File doesn't exist or cannot be opened\n");
        return;
    } else {
        fscanf(fp, " "); /*skip whitespaces*/
        fscanf(fp, "%d", &ROWS_PER_BLOCK);
        fscanf(fp, " "); /*skip whitespaces*/
        fscanf(fp, "%d", &COLUMNS_PER_BLOCK);
        init_game();
        fscanf(fp, " "); /*skip whitespaces*/
        for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
            for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                fscanf(fp, "%d", &(game_board[i][j]));
                if (feof(fp)) {
                    break;
                } else if ((c = fgetc(fp)) == '.') {
                    fixed_numbers_board[i][j] = 1;
                    fscanf(fp, " "); /*skip whitespaces*/
                } else {
                    fscanf(fp, " "); /*skip whitespaces*/
                }
            }
        }
    }
    fclose(fp);
}

void edit(char *path) {
    FILE *fp;
    int i;
    int j;
    int c;
    GAME_MODE = 1; /*Edit mode*/
    if (path == NULL) {
        //create an empty 9x9 board
        ROWS_PER_BLOCK = 3;
        COLUMNS_PER_BLOCK = 3;
        init_game();
    } else {
        // loads the board from a file
        fp = fopen(path, "r");
        if (fp == NULL) {
            printf("Error: File doesn't exist or cannot be opened\n");
            return;
        } else {
            fscanf(fp, " "); /*skip whitespaces*/
            fscanf(fp, "%d", &ROWS_PER_BLOCK);
            fscanf(fp, " "); /*skip whitespaces*/
            fscanf(fp, "%d", &COLUMNS_PER_BLOCK);
            init_game();
            fscanf(fp, " "); /*skip whitespaces*/
            for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
                for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                    fscanf(fp, "%d", &(game_board[i][j]));
                    if (feof(fp)) {
                        break;
                    } else if ((c = fgetc(fp)) == '.') {
                        fixed_numbers_board[i][j] = 1;
                        fscanf(fp, " "); /*skip whitespaces*/
                    } else {
                        fscanf(fp, " "); /*skip whitespaces*/
                    }
                }
            }
        }
        fclose(fp);
    }
}

void mark_errors(int X) {
    MARK_ERRORS = X;
}

void free_next_moves() {
    struct game_move *current;
    struct game_move *head;
    current = game_moves;
    head = game_moves;
    while ((*current).next != NULL) {
        current = (*current).next;
        free(head);
        head = current;
    }
}

void update_moves_list(int x, int y, int z, int old) {
    struct game_move *last_move;
    if (game_moves == NULL) {
        // initializing moves list
        game_moves = (struct game_move *) malloc(sizeof(struct game_move));
        (*game_moves).prev = NULL;
        (*game_moves).next = NULL;
        (*game_moves).y_value = 0;
        (*game_moves).x_value = 0;
        (*game_moves).new_z_value = 0;
        (*game_moves).old_z_value = 0;
    }
    last_move = game_moves;
    free_next_moves();
    (*game_moves).next = (struct game_move *) malloc(sizeof(struct game_move));
    game_moves = (*game_moves).next;
    (*game_moves).x_value = x;
    (*game_moves).y_value = y;
    (*game_moves).new_z_value = z;
    (*game_moves).old_z_value = old;
    (*game_moves).prev = last_move;
    (*game_moves).next = NULL;
}

void set_cell(int x, int y, int z) {
    int old;
    if (x < 0 || y < 0 || x >= ROWS_COLUMNS_NUM || y >= ROWS_COLUMNS_NUM || z < 0 || z > ROWS_COLUMNS_NUM) {
        printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
        return;
    }
    if (fixed_numbers_board[x][y] == 1) {
        printf("Error: cell is fixed\n");
        return;
    }
    old = game_board[x][y];
    game_board[x][y] = z;
    EMPTY_CELLS_NUM--;
    update_moves_list(x, y, z, old);
    if (MARK_ERRORS) {
        if (!validate_solution()) {
            erroneous_board[x][y] = 1;
        }
    }
    if (EMPTY_CELLS_NUM == 0) {
        if (!validate_solution()) {
            printf("Puzzle solution erroneous\n");
            return;
        } else {
            printf("Puzzle solved successfully\n");
            GAME_MODE = 0;
        }
    }
}


void undo() {
    if ((*game_moves).prev == NULL) {
        printf("Error: no moves to undo\n");
        return;
    }
    if ((*game_moves).old_z_value == 0) {
        printf("Undo %d,%d: from %d to _\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).new_z_value);
        EMPTY_CELLS_NUM++;
    } else if ((*game_moves).new_z_value == 0) {
        printf("Undo %d,%d: from _ to %d\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).old_z_value);
        EMPTY_CELLS_NUM--;
    } else {
        printf("Undo %d,%d: from %d to %d\n", (*game_moves).y_value + 1, (*game_moves).x_value + 1,
               (*game_moves).new_z_value, (*game_moves).old_z_value);
    }
    game_board[(*game_moves).x_value][(*game_moves).y_value] = (*game_moves).old_z_value;
    game_moves = (*game_moves).prev;
}

void redo() {
    int new_value;
    if ((*game_moves).next == NULL) {
        printf("Error: no moves to redo\n");
        return;
    }
    new_value = (*(*game_moves).next).new_z_value;
    if ((*(*game_moves).next).new_z_value == 0) {
        printf("Redo %d,%d: from %d to _\n", (*(*game_moves).next).y_value + 1, (*(*game_moves).next).x_value + 1,
               (*(*game_moves).next).old_z_value);
        EMPTY_CELLS_NUM++;
    } else if ((*(*game_moves).next).old_z_value == 0) {
        printf("Redo %d,%d: from _ to %d\n", (*(*game_moves).next).y_value + 1, (*(*game_moves).next).x_value + 1,
               (*(*game_moves).next).new_z_value);
        EMPTY_CELLS_NUM--;
    } else {
        printf("Redo %d,%d: from %d to %d\n", (*(*game_moves).next).y_value + 1, (*(*game_moves).next).x_value + 1,
               (*(*game_moves).next).new_z_value,
               (*(*game_moves).next).old_z_value);
    }
    game_board[(*(*game_moves).next).x_value][(*(*game_moves).next).y_value] = new_value;
    game_moves = (*game_moves).next;
}

void save_board(char *path) {
    FILE *fp;
    char *size_to_write;
    int i;
    int j;
    /*save is only available in Edit and Solve modes*/
    if ((GAME_MODE != 1) && (GAME_MODE != 2)) {
        printf("ERROR: invalid command\n");
    } else if (GAME_MODE == 1) {
        /*Edit mode*/
        for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
            for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                if (erroneous_board[i][j] == 1) {
                    printf("ERROR: board contains erroneous values\n");
                    return;
                }
            }
        }
        if (validate_solution() == 0) {
            printf("ERROR: board validation failed\n");
            return;
        }
    }
    fp = fopen(path, "w+");
    if (fp == NULL) {
        printf("Error: File cannot be created or modified\n");
        return;
    } else {
        /*save game_board to file*/
        fprintf(fp, "%d", ROWS_PER_BLOCK);
        fprintf(fp, " ");
        fprintf(fp, "%d", COLUMNS_PER_BLOCK);
        fprintf(fp, "\n");
        for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
            for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
                fprintf(fp, "%d", game_board[i][j]);
                if ((GAME_MODE == 1) && (game_board[i][j] != 0)) {
                    /*in Edit mode, all cells containing values are marked as "fixed"*/
                    fixed_numbers_board[i][j] = 1;
                    fprintf(fp, ".");
                } else if (fixed_numbers_board[i][j] == 1) {
                    fprintf(fp, ".");
                }
                fprintf(fp, " ");
            }
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
    printf("Saved to: %s\n", path);
}

void get_hint(int x, int y) {

}

int check_if_board_erroneous() {
    int i;
    int j;
    int x;
    int y;
    int number;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            number = game_board[i][j];
            if (!(number_does_not_exist_in_row(number, x) && number_does_not_exist_in_column(number, y) &&
                  number_does_not_exist_in_block(number, x, y))) {
                return 1;
            }
        }
    }
    return 0;
}

int validate_solution() {
    if (check_if_board_erroneous()) {
        printf("Validation failed: board is unsolvable\n");
        return 0;
    } else if (solve_board(game_board, ROWS_COLUMNS_NUM, ROWS_PER_BLOCK, COLUMNS_PER_BLOCK)) {
        printf("Validation passed: board is solvable\n");
        return 1;
    } else {
        printf("Validation failed: board is unsolvable\n");
        return 0;
    }
}

void clear_board(){
    int i;
    int j;
    for (i=0; i<ROWS_COLUMNS_NUM; i++){
        for (j=0; j<ROWS_COLUMNS_NUM; j++){
            game_board[i][j] = 0;
            erroneous_board[i][j] = 0;
            fixed_numbers_board[i][j] = 0;
        }
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
    count = 0;
    legal_values = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    for (i = 0; i < x; i++) {
        x_index = rand() % ROWS_COLUMNS_NUM;
        y_index = rand() % ROWS_COLUMNS_NUM;
        get_available_numbers_for_set(legal_values, x_index, y_index, game_board);
        for (j=0; j<ROWS_COLUMNS_NUM; j++){
            if (legal_values[j]>0){
                count++;
            }
        }
        if (count == 0) {
            clear_board();
            return 0;
        }
        rand_value = rand() % count;
        j = 0;
        while (rand_value>0){
            if (legal_values[i]==0){
                count--;
            }
            j++;
        }
        game_board[x_index][y_index] = legal_values[j];
    }
    free(legal_values);
    if (!solve_board(game_board, ROWS_COLUMNS_NUM, ROWS_PER_BLOCK, COLUMNS_PER_BLOCK)) {
        return 0;
    }
    return 1;
}

void generate(int x, int y) {
    int i;
    int x_index;
    int y_index;
    int **tmp_board;
    tmp_board = (int **)malloc(sizeof(int*)*ROWS_COLUMNS_NUM);
    for (i=0; i<ROWS_COLUMNS_NUM; i++){
        tmp_board[i] = (int *)malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    }
    if (EMPTY_CELLS_NUM < ROWS_COLUMNS_NUM * ROWS_COLUMNS_NUM) {
        printf("Error: board is not empty\n"); // TODO: is it a space before the \n in the instructions document?
        return;
    }
    if (x > EMPTY_CELLS_NUM) {
        printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
        return;
    }
    i = 0;
    while (!try_generate(x) && i < 1000) {
        i++;
    }
    if (i>=1000){
        printf("Error: puzzle generator failed\n");
        return;
    }
    // if the program got here, it means that the board was generated successfully
    copy_board(solved_board, tmp_board, 0);
    for (i = 0; i < y; i++) {
        x_index = rand() % ROWS_COLUMNS_NUM;
        y_index = rand() % ROWS_COLUMNS_NUM;
        tmp_board[x_index][y_index] = 0;
    }
    copy_board(tmp_board, game_board, 1);
}

void num_solutions() {
    int ans;
    if(check_if_board_erroneous() == 1){
        printf("Error: board contains erroneous values\n");
        return;
    }
    ans = exhaustive_backtracking(0, 0, game_board, 0); //TODO: correct parameters?
    printf("Number of solutions: %d\n", ans);
    if(ans == 1){
        printf("This is a good board!\n");
    }
    else{
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
}

int number_does_not_exist_in_row(int number, int row) {
    int i;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (game_board[row][i] == number) {
            return 0;
        }
    }
    return 1;
}

int number_does_not_exist_in_column(int number, int column) {
    int i;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (game_board[i][column] == number) {
            return 0;
        }
    }
    return 1;
}

int number_does_not_exist_in_block(int number, int row, int column) {
    int i;
    int j;
    int row_lower_bound;
    int row_upper_bound;
    int column_lower_bound;
    int column_upper_bound;
    row_lower_bound = (row / ROWS_PER_BLOCK);
    row_upper_bound = row_lower_bound + ROWS_PER_BLOCK;
    column_lower_bound = (column / COLUMNS_PER_BLOCK);
    column_upper_bound = column_lower_bound + COLUMNS_PER_BLOCK;
    for (i = row_lower_bound; i < row_upper_bound; i++) {
        for (j = column_lower_bound; j < column_upper_bound; j++) {
            if (game_board[i][j] == number) {
                return 0;
            }
        }
    }
    return 1;
}

void get_available_numbers_for_set(int available_numbers[ROWS_COLUMNS_NUM], int rows_index, int columns_index,
                                   int new_solved_board[ROWS_COLUMNS_NUM][ROWS_COLUMNS_NUM]) {
    int i;
    int j;
    int block_first_row;
    int block_first_column;
    int block_last_row;
    int block_last_column;
    /* first we check for the row*/
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (new_solved_board[rows_index][i] != 0) {
            if (new_solved_board[rows_index][i] != 0) {
                available_numbers[new_solved_board[rows_index][i] - 1]++;
            }
        }
    }
    /* then we check the column*/
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (new_solved_board[i][columns_index] != 0) {
            available_numbers[new_solved_board[i][columns_index] - 1]++;
        }
    }
    /* and last we check the block*/
    block_first_row = rows_index - rows_index % ROWS_PER_BLOCK;
    block_last_row = block_first_row + ROWS_PER_BLOCK;
    block_first_column = columns_index - columns_index % COLUMNS_PER_BLOCK;
    block_last_column = block_first_column + COLUMNS_PER_BLOCK;
    for (i = block_first_row; i < block_last_row; i++) {
        for (j = block_first_column; j < block_last_column; j++) {
            if (new_solved_board[i][j] != 0 && rows_index != i && columns_index != j) {
                available_numbers[new_solved_board[i][j] - 1]++;
            }
        }
    }
}

int get_autofill_value(int x, int y) {
    int i;
    int available_number;
    int count; // counts the number of available numbers that can be filled in the empty cell.
    count = 0;
    for (i = 1; i <= ROWS_COLUMNS_NUM; i++) {
        // iterating all possible numbers
        if (number_does_not_exist_in_row(i, x) && number_does_not_exist_in_column(i, y) &&
            number_does_not_exist_in_block(i, x, y)) {
            count++;
            available_number = i;
        }
    }
    if (count > 1 || count == 0) {
        return 0;
    } else if (count == 1) {
        return available_number;
    }
}


void autofill() {
    int i;
    int j;
    int new_value;
    int **filled_board;
    if (!validate_solution()) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    filled_board = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    if (filled_board == NULL) {
        printf("Error: autofill failed\n");
        return;
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        filled_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        if (filled_board[i] == NULL) {
            printf("Error: autofill failed\n");
            return;
        }
    }
    copy_board(game_board, filled_board, 0);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (game_board[i][j] == 0) {
                // we are going to check if a cell has an obvious value only if the cell is empty
                if ((new_value = get_autofill_value(i, j)) > 0) {
                    // means that there is an autofill option
                    filled_board[i][j] = new_value; // we do not want the new values to affect autofill.
                }
            }
        }
    }
    copy_board(filled_board, game_board, 1);
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        free(filled_board[i]);
    }
    free(filled_board);
}

void clear_moves_list_from_first() {
    // this function assumes game_moves have no prev value (the current move is the first move in the list)
    struct game_move *current;
    while ((*game_moves).next != NULL) {
        current = game_moves;
        game_moves = (*game_moves).next;
        free(current);
    }
}

void restart_game() {
    while ((*game_moves).prev != NULL) {
        undo();
    }
    clear_moves_list_from_first();
    printf("Board reset\n");
}

void exit_game() {
    printf("Exiting...\n");
    while ((*game_moves).prev != NULL) {
        game_moves = (*game_moves).prev;
    }
    free_next_moves();
    // TODO: check what else should be freed here
}

void init_game() {
    int i;
    int j;
    ROWS_COLUMNS_NUM = ROWS_PER_BLOCK * COLUMNS_PER_BLOCK;
    EMPTY_CELLS_NUM = ROWS_COLUMNS_NUM * ROWS_COLUMNS_NUM;
    game_board = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    erroneous_board = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    fixed_numbers_board = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
    if (game_board == NULL || erroneous_board == NULL || fixed_numbers_board == NULL) {
        printf("Error: game initialization failed\n"); // TODO: validate that this is the message that should be printed
        return;
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        game_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        fixed_numbers_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        erroneous_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        if (game_board[i] == NULL || erroneous_board[i] == NULL || fixed_numbers_board[i] == NULL) {
            printf("Error: game initialization failed\n"); // TODO: validate that this is the message that should be printed
            return;
        }
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            game_board[i][j] = 0;
            fixed_numbers_board[i][j] = 0;
            erroneous_board[i][j] = 0;
        }
    }
    srand(time(NULL)); // setting seed for random
}