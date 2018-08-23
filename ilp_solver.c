#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ilp_solver.h"
#include "gurobi_c.h"

void create_empty_model(int **game_board, double *lb, char *vtype, int rows_columns) {
    int i;
    int j;
    int v;
    for (i = 0; i < rows_columns; i++) {
        for (j = 0; j < rows_columns; j++) {
            for (v = 0; v < rows_columns; v++) {
                if ((game_board[i][j] - 1) == v)
                    /* we may look at the 3D matrix in the following way:
                     * matrix i represents all possible values for row i in the original game board while
                     * matrix[i,j] is a boolean array of size rows_columns represents all possible values for game_board[i,j].
                     * */
                    lb[i * rows_columns * rows_columns + j * rows_columns + v] = 1;
                else
                    lb[i * rows_columns * rows_columns + j * rows_columns + v] = 0;
                vtype[i * rows_columns * rows_columns + j * rows_columns + v] = GRB_BINARY;
            }
        }
    }
}

void allocate_ilp_variables(int **ind, double **val, double **lb, char **vtype, int rows_columns) {
    (*ind) = (int *) malloc(rows_columns * sizeof(int));
    (*val) = (double *) malloc(rows_columns * sizeof(double));
    (*lb) = (double *) malloc(rows_columns * rows_columns * rows_columns * sizeof(double));
    (*vtype) = (char *) malloc(rows_columns * rows_columns * rows_columns * sizeof(char));
}


void free_ilp_variables(int **ind, double **val, double **lb, char **vtype) {
    free(*ind);
    free(*val);
    free(*lb);
    free(*vtype);
}

int solve_board(int **game_board, int rows_columns, int rows_per_block, int columns_per_block,
                int fill_solved_board_with_solution, int **solved_board) {
    return 1;
}



