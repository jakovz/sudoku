#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "helper_functions.h"

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

/* Credit: https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c */
char **str_split(char *a_str, const char a_delim, size_t *count) {
    char **result = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    *count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    (*count)++;

    result = malloc(sizeof(char *) * (*count));

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void get_available_numbers_for_set(int *available_numbers, int rows_index, int columns_index,
                                   int **new_solved_board, int rows_colummns_num, int rows_per_block,
                                   int columns_per_block) {
    int i;
    int j;
    int block_first_row;
    int block_first_column;
    int block_last_row;
    int block_last_column;
    /* first we check for the row*/
    for (i = 0; i < rows_colummns_num; i++) {
        if (new_solved_board[rows_index][i] != 0) {
            if (new_solved_board[rows_index][i] != 0) {
                available_numbers[new_solved_board[rows_index][i] - 1]++;
            }
        }
    }
    /* then we check the column*/
    for (i = 0; i < rows_colummns_num; i++) {
        if (new_solved_board[i][columns_index] != 0) {
            available_numbers[new_solved_board[i][columns_index] - 1]++;
        }
    }
    /* and last we check the block*/
    block_first_row = rows_index - rows_index % rows_per_block;
    block_last_row = block_first_row + rows_per_block;
    block_first_column = columns_index - columns_index % columns_per_block;
    block_last_column = block_first_column + columns_per_block;
    for (i = block_first_row; i < block_last_row; i++) {
        for (j = block_first_column; j < block_last_column; j++) {
            if (new_solved_board[i][j] != 0 && rows_index != i && columns_index != j) {
                available_numbers[new_solved_board[i][j] - 1]++;
            }
        }
    }
}

int number_does_not_exist_in_row(int **game_board, int number, int row, int column, int rows_columns_num) {
    int i;
    for (i = 0; i < rows_columns_num; i++) {
        if (game_board[row][i] == number && i!=column) {
            return 0;
        }
    }
    return 1;
}

int number_does_not_exist_in_column(int **game_board, int number, int row, int column, int rows_columns_num) {
    int i;
    for (i = 0; i < rows_columns_num; i++) {
        if (game_board[i][column] == number && i!=row) {
            return 0;
        }
    }
    return 1;
}

int number_does_not_exist_in_block(int **game_board, int number, int row, int column, int rows_per_block, int columns_per_block) {
    int i;
    int j;
    int row_lower_bound;
    int row_upper_bound;
    int column_lower_bound;
    int column_upper_bound;
    row_lower_bound = (row / rows_per_block)*rows_per_block;
    row_upper_bound = row_lower_bound + rows_per_block;
    column_lower_bound = (column / columns_per_block)*columns_per_block;
    column_upper_bound = column_lower_bound + columns_per_block;
    for (i = row_lower_bound; i < row_upper_bound; i++) {
        for (j = column_lower_bound; j < column_upper_bound; j++) {
            if (game_board[i][j] == number && i!=row && j!=column) {
                return 0;
            }
        }
    }
    return 1;
}

int number_is_available(int **game_board, int number, int row, int column, int rows_per_block, int columns_per_block){
    if (number_does_not_exist_in_row(game_board, number, row, column, rows_per_block*columns_per_block) && number_does_not_exist_in_column(game_board, number, row, column, rows_per_block*columns_per_block) &&
        number_does_not_exist_in_block(game_board, number, row, column, rows_per_block, columns_per_block)){
        return 1;
    }
    return 0;
}
