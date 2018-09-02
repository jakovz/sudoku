#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_board_operations.h"
#include "game_moves_list.h"

void read_board_from_file(FILE *file_descriptor) {
    int i;
    int j;
    fscanf(file_descriptor, " "); /*skip whitespaces*/
    fscanf(file_descriptor, "%d", &ROWS_PER_BLOCK);
    fscanf(file_descriptor, " "); /*skip whitespaces*/
    fscanf(file_descriptor, "%d", &COLUMNS_PER_BLOCK);
    init_game();
    fscanf(file_descriptor, " "); /*skip whitespaces*/
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            fscanf(file_descriptor, "%d", &(game_board[i][j]));
            if (feof(file_descriptor)) {
                break;
            } else if (fgetc(file_descriptor) == '.') {
                fixed_numbers_board[i][j] = 1;
                fscanf(file_descriptor, " "); /*skip whitespaces*/
            } else {
                fscanf(file_descriptor, " "); /*skip whitespaces*/
            }
        }
    }
}

void save_board_to_file(FILE *file_descriptor) {
    int i;
    int j;
    fprintf(file_descriptor, "%d", ROWS_PER_BLOCK);
    fprintf(file_descriptor, " ");
    fprintf(file_descriptor, "%d", COLUMNS_PER_BLOCK);
    fprintf(file_descriptor, "\n");
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            fprintf(file_descriptor, "%d", game_board[i][j]);
            if ((GAME_MODE == 1) && (game_board[i][j] != 0)) {
                /* in Edit mode, all cells containing values are marked as "fixed" */
                fixed_numbers_board[i][j] = 1;
                fprintf(file_descriptor, ".");
            } else if (fixed_numbers_board[i][j] == 1) {
                fprintf(file_descriptor, ".");
            }
            fprintf(file_descriptor, " ");
        }
        fprintf(file_descriptor, "\n");
    }
}

void copy_board(int **from, int **to, int save_moves) {
    /* setting save moves to 1 or bigger will make undo treat all set commands committed as one command */
    int i;
    int j;
    int old;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (save_moves) {
                if (from[i][j] != 0 && from[i][j] != to[i][j]) {
                    old = game_board[i][j];
                    game_board[i][j] = from[i][j];
                    EMPTY_CELLS_NUM--;
                    update_moves_list(i, j, from[i][j], old, 1, 0, 0);
                }
            } else {
                to[i][j] = from[i][j];
            }
        }
    }
}

int number_does_not_exist_in_row(int number, int row, int column) {
    int i;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (game_board[row][i] == number && i != column) {
            erroneous_board[row][i] = 1;
            /*in order to mark all erroneous cells as so*/
            erroneous_board[row][column] = 1;
            return 0;
        }
    }
    return 1;
}

int number_does_not_exist_in_column(int number, int row, int column) {
    int i;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        if (game_board[i][column] == number && i != row) {
            erroneous_board[i][column] = 1;
            erroneous_board[row][column] = 1; /*in order to mark all erroneous cells as so*/
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
    row_lower_bound = (row / ROWS_PER_BLOCK) * ROWS_PER_BLOCK;
    row_upper_bound = row_lower_bound + ROWS_PER_BLOCK;
    column_lower_bound = (column / COLUMNS_PER_BLOCK) * COLUMNS_PER_BLOCK;
    column_upper_bound = column_lower_bound + COLUMNS_PER_BLOCK;
    for (i = row_lower_bound; i < row_upper_bound; i++) {
        for (j = column_lower_bound; j < column_upper_bound; j++) {
            if (game_board[i][j] == number && i != row && j != column) {
                erroneous_board[i][j] = 1;
                erroneous_board[row][column] = 1; /*in order to mark all erroneous cells as so*/
                return 0;
            }
        }
    }
    return 1;
}

int number_is_available(int number, int row, int column) {
    if (number_does_not_exist_in_row(number, row, column) &&
        number_does_not_exist_in_column(number, row, column) &&
        number_does_not_exist_in_block(number, row, column)) {
        return 1;
    }
    return 0;
}

int check_if_board_erroneous() {
    int i;
    int j;
    int number;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            number = game_board[i][j];
            if (number != 0 && !number_is_available(number, i, j)) {
                return 1;
            }
        }
    }
    return 0;
}

void free_game_boards() {
    int i;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        free(game_board[i]);
        free(fixed_numbers_board[i]);
        free(erroneous_board[i]);
        free(solved_board[i]);
    }
    free(game_board);
    free(erroneous_board);
    free(fixed_numbers_board);
    free(solved_board);
}

int count_empty_cells() {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            if (game_board[i][j] != 0) {
                count++;
            }
        }
    }
    return (ROWS_COLUMNS_NUM * ROWS_COLUMNS_NUM) - count; /* number of total cells minus the number of filled cells */
}

void init_game() {
    int i;
    int j;
    MARK_ERRORS = 1;
    if (GAME_ALREADY_INITIALIZED) {
        /* making sure no memory is left allocated from the previous allocation */
        free_game_boards();
        clear_moves_list();
    }
    initialize_game_moves_list();
    ROWS_COLUMNS_NUM = ROWS_PER_BLOCK * COLUMNS_PER_BLOCK;
    EMPTY_CELLS_NUM = ROWS_COLUMNS_NUM * ROWS_COLUMNS_NUM;
    game_board = malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    erroneous_board = malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    fixed_numbers_board = malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    solved_board = malloc(sizeof(int *) * ROWS_COLUMNS_NUM);
    if (game_board == NULL || erroneous_board == NULL || fixed_numbers_board == NULL || solved_board == NULL) {
        printf("Error: game initialization failed\n");
        exit(-1);
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        game_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        fixed_numbers_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        erroneous_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        solved_board[i] = malloc(sizeof(int) * ROWS_COLUMNS_NUM);
        if (game_board[i] == NULL || erroneous_board[i] == NULL || fixed_numbers_board[i] == NULL ||
            solved_board[i] == NULL) {
            printf("Error: game initialization failed\n");
            exit(-1);
        }
    }
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            game_board[i][j] = 0;
            fixed_numbers_board[i][j] = 0;
            erroneous_board[i][j] = 0;
            solved_board[i][j] = 0;
        }
    }
    GAME_ALREADY_INITIALIZED = 1;
    srand((unsigned int) time(NULL)); /* setting seed for random */
}

void clear_game_boards() {
    int i;
    int j;
    for (i = 0; i < ROWS_COLUMNS_NUM; i++) {
        for (j = 0; j < ROWS_COLUMNS_NUM; j++) {
            game_board[i][j] = 0;
            erroneous_board[i][j] = 0;
            fixed_numbers_board[i][j] = 0;
        }
    }

}

void get_available_numbers_for_set(int *available_numbers, int rows_index, int columns_index) {
    int i;
    for (i = 1; i <= ROWS_COLUMNS_NUM; i++) {
        if (number_is_available(i, rows_index, columns_index)) {
            available_numbers[i - 1] = 0;
        } else {
            available_numbers[i - 1] = 1;
        }
    }
}

void print_board() {
    int i;
    int j;
    int z;
    int rows_counter;
    int columns_counter;
    rows_counter = 0;
    if (ROWS_PER_BLOCK == 0 && COLUMNS_PER_BLOCK == 0) {
        /*  means the board is empty and therefore there is nothing to print */
        return;
    }
    for (i = 0; i < (ROWS_PER_BLOCK * COLUMNS_PER_BLOCK + COLUMNS_PER_BLOCK + 1); i++) {
        /* ROWS_PER_BLOCK*COLUMNS_PER_BLOCK + COLUMNS_PER_BLOCK is overall number of rows (including separator rows) */
        /* ROWS_PER_BLOCK = m = number of rows of cells per block */
        if ((i % (ROWS_PER_BLOCK + 1)) == 0) {
            /* separator row */
            for (z = 0; z < (4 * (ROWS_PER_BLOCK * COLUMNS_PER_BLOCK) + ROWS_PER_BLOCK); z++) {
                printf("-");
            }
            printf("-\n");
            rows_counter++;
        } else {
            columns_counter = 0;
            for (j = 0; j < (ROWS_PER_BLOCK * COLUMNS_PER_BLOCK + ROWS_PER_BLOCK + 1); j++) {
                /* COLUMNS_PER_BLOCK = n = number of rows of blocks */
                if ((j % (COLUMNS_PER_BLOCK + 1)) == 0) {
                    if (j == ROWS_PER_BLOCK * COLUMNS_PER_BLOCK + ROWS_PER_BLOCK) {
                        printf("|\n");
                    } else {
                        printf("|");
                    }
                    columns_counter++;
                } else {
                    printf(" ");
                    if (game_board[i - rows_counter][j - columns_counter] == 0) {
                        printf("  ");
                    } else {
                        printf("%2d", game_board[i - rows_counter][j - columns_counter]);
                    }
                    if (fixed_numbers_board[i - rows_counter][j - columns_counter] == 1 && GAME_MODE != 1) {
                        /*means that this is a fixed cell*/
                        printf(".");
                    } else if (GAME_MODE == 1 || MARK_ERRORS == 1) {
                        if (erroneous_board[i - rows_counter][j - columns_counter] == 1) {
                            /*means that this is an erroneous value*/
                            printf("*");
                        } else {
                            printf(" ");
                        }
                    } else {
                        printf(" ");
                    }
                }
            }
        }
    }
}