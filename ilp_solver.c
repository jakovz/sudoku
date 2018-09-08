#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ilp_solver.h"
#include "gurobi_c.h"

static void create_empty_model(int **game_board, double *lb, char *vtype, int rows_columns) {
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

static int create_single_value_per_cell_constraints(GRBmodel *model, int *ind, double *val, int error, int rows_columns) {
    int i;
    int j;
    int v;
    for (i = 0; i < rows_columns; i++) {
        for (j = 0; j < rows_columns; j++) {
            for (v = 0; v < rows_columns; v++) {
                ind[v] = i * rows_columns * rows_columns + j * rows_columns + v;
                val[v] = 1.0;
            }

            error = GRBaddconstr(model, rows_columns, ind, val, GRB_EQUAL, 1.0, NULL);
        }
    }
    return error;
}

static int create_appear_once_in_row_constrains(GRBmodel *model, int *ind, double *val, int error,
                                         int rows_columns) {
    int v;
    int j;
    int i;
    for (v = 0; v < rows_columns; v++) {
        for (j = 0; j < rows_columns; j++) {
            for (i = 0; i < rows_columns; i++) {
                ind[i] = i * rows_columns * rows_columns + j * rows_columns + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, rows_columns, ind, val, GRB_EQUAL, 1.0, NULL);
        }
    }
    return error;
}

static int create_appear_once_in_column_constrains(GRBmodel *model, int *ind, double *val, int error, int rows_columns) {
    int v;
    int i;
    int j;
    for (v = 0; v < rows_columns; v++) {
        for (i = 0; i < rows_columns; i++) {
            for (j = 0; j < rows_columns; j++) {
                ind[j] = i * rows_columns * rows_columns + j * rows_columns + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, rows_columns, ind, val, GRB_EQUAL, 1.0, NULL);
        }
    }
    return error;
}

static int
create_appear_once_in_block_constrains(GRBmodel *model, int *ind, double *val, int count, int error, int rows_columns,
                                       int rows_per_block, int columns_per_block) {
    int v, i, j, ig, jg;
    for (v = 0; v < rows_columns; v++) {
        for (jg = 0; jg < rows_per_block; jg++) {
            for (ig = 0; ig < columns_per_block; ig++) {
                count = 0;
                for (i = ig * rows_per_block; i < (ig + 1) * rows_per_block; i++) {
                    for (j = jg * columns_per_block; j < (jg + 1) * columns_per_block; j++) {
                        ind[count] = i * rows_columns * rows_columns + j * rows_columns + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr(model, rows_columns, ind, val, GRB_EQUAL, 1.0, NULL);
            }
        }
    }
    return error;
}

static void allocate_ilp_variables(int **ind, double **val, double **lb, char **vtype, int rows_columns) {
    (*ind) = (int *) malloc(rows_columns * sizeof(int));
    (*val) = (double *) malloc(rows_columns * sizeof(double));
    (*lb) = (double *) malloc(rows_columns * rows_columns * rows_columns * sizeof(double));
    (*vtype) = (char *) malloc(rows_columns * rows_columns * rows_columns * sizeof(char));
    if ((*ind)==NULL || (*val)==NULL || (*lb)==NULL || (*vtype)==NULL){
        printf("Error: ILP has failed\n");
        exit(-1);
    }
}


static void free_ilp_variables(int **ind, double **val, double **lb, char **vtype) {
    free(*ind);
    free(*val);
    free(*lb);
    free(*vtype);
}

int solve_board(int **game_board, int rows_columns, int rows_per_block, int columns_per_block,
                int fill_solved_board_with_solution, int **solved_board) {
    GRBenv *env;
    GRBmodel *model;
    int *ind;
    double *val;
    double *lb;
    char *vtype;
    int optimstatus;
    int count;
    int error;
    int result;
    double *results;
    int i;
    int j;
    int v;
    count = 0;
    result = 0;
    env = NULL;
    model = NULL;
    allocate_ilp_variables(&ind, &val, &lb, &vtype, rows_columns);
    create_empty_model(game_board, lb, vtype, rows_columns); /* creating an empty model */
    error = GRBloadenv(&env, NULL); /* creating an environment */
    if (error) goto QUIT;

    error = GRBsetintparam(env, GRB_INT_PAR_OUTPUTFLAG, 0); /* for us to not see gurobi output messages */
    if (error) goto QUIT;

    error = GRBnewmodel(env, &model, "sudoku", rows_columns * rows_columns * rows_columns, NULL, lb, NULL, vtype,
                        NULL); /* creating a new model */
    if (error) goto QUIT;

    error = create_single_value_per_cell_constraints(model, ind, val, error, rows_columns); /* Each cell gets a value */
    if (error) goto QUIT;

    error = create_appear_once_in_row_constrains(model, ind, val, error,
                                                 rows_columns); /* Each value must appear once in each row */
    if (error) goto QUIT;

    /* Each value must appear once in each column */
    error = create_appear_once_in_column_constrains(model, ind, val, error, rows_columns);
    if (error) goto QUIT;

    /* Each value must appear once in each block */
    error = create_appear_once_in_block_constrains(model, ind, val, count, error, rows_columns, rows_per_block,
                                                   columns_per_block);
    if (error) goto QUIT;

    error = GRBoptimize(model); /* optimizing model */
    if (error) goto QUIT;

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        result = 1; /* success */
    } else {
        result = 0;
        free_ilp_variables(&ind, &val, &lb, &vtype);
        GRBfreemodel(model);  /* free model */
        GRBfreeenv(env); /* free environment */
        return result;
    }

    results = (double *) malloc(sizeof(double) * rows_columns * rows_columns * rows_columns);
    if (results == NULL){
        printf("Error: ILP has failed\n");
        exit(-1);
    }

    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, rows_columns * rows_columns * rows_columns, results);
    if (error) goto QUIT;

    if (fill_solved_board_with_solution) {
        for (i = 0; i < rows_columns; i++) {
            for (j = 0; j < rows_columns; j++) {
                for (v = 0; v < rows_columns; v++) {
                    if ((int) results[i * rows_columns * rows_columns + j * rows_columns + v] == 1) {
                        solved_board[i][j] = v + 1;
                    }
                }
            }
        }
    }

    QUIT:
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env)); /* reporting the error */
        exit(-1);
    }
    free(results);
    free_ilp_variables(&ind, &val, &lb, &vtype);
    GRBfreemodel(model);  /* free model */
    GRBfreeenv(env); /* free environment */
    return result;
}



