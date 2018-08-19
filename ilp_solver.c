#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"

void create_empty_model(int **game_board, double *lb, char *vtype, char **names, char *namestorage, char *cursor, int rows_columns) {
    int i;
    int j;
    int v;
    cursor = namestorage;
    for (i = 0; i < rows_columns; i++) {
        for (j = 0; j < rows_columns; j++) {
            for (v = 0; v < rows_columns; v++) {
                if (game_board[i][j] == v)
                    lb[i * rows_columns * rows_columns + j * rows_columns + v] = 1;
                else
                    lb[i * rows_columns * rows_columns + j * rows_columns + v] = 0;
                vtype[i * rows_columns * rows_columns + j * rows_columns + v] = GRB_BINARY;

                 [i * rows_columns * rows_columns + j * rows_columns + v] = cursor;
                sprintf(names[i * rows_columns * rows_columns + j * rows_columns + v], "x[%d,%d,%d]", i, j,
                        v + 1);
                cursor += strlen(names[i * rows_columns * rows_columns + j * rows_columns + v]) + 1;
            }
        }
    }
}

int create_single_value_per_cell_constraints(GRBmodel *model, int *ind, double *val, int error, int rows_columns) {
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

int create_appear_once_in_row_constrains(GRBmodel *model, int *ind, double *val, int error,
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

int create_appear_once_in_column_constrains(GRBmodel *model, int *ind, double *val, int error, int rows_columns) {
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

int
create_appear_once_in_block_constrains(GRBmodel *model, int *ind, double *val, int count, int error, int rows_columns,
                                       int rows_per_block, int columns_per_block) {
    int v, i, j, ig, jg;
    for (v = 0; v < rows_columns; v++) {
        for (ig = 0; ig < rows_per_block; ig++) {
            for (jg = 0; jg < columns_per_block; jg++) {
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

void allocate_ilp_variables(int **ind, double **val, double **lb, char **vtype, char ***names, char **namestorage, int rows_columns) {
    (*ind) = (int *) malloc(rows_columns * sizeof(int));
    (*val) = (double *) malloc(rows_columns * sizeof(double));
    (*lb) = (double *) malloc(rows_columns * rows_columns * rows_columns * sizeof(double));
    (*vtype) = (char *) malloc(rows_columns * rows_columns * rows_columns * sizeof(char));
    (*names) = (char **) malloc(rows_columns * rows_columns * rows_columns * sizeof(char *));
    (*namestorage) = (char *) malloc(rows_columns * rows_columns * rows_columns * 10 * sizeof(char)); // TODO: why 10?

}


void free_ilp_variables(int *ind, double *val, double *lb, char *vtype, char **names,
                        char *namestorage) {
    free(ind);
    free(val);
    free(lb);
    free(vtype);
    free(names);
    free(namestorage);
}

int solve_board(int **game_board, int rows_columns, int rows_per_block, int columns_per_block) {
    GRBenv *env;
    GRBmodel *model;
    int *ind;
    double *val;
    double *lb;
    char *vtype;
    char **names;
    char *namestorage;
    char *cursor;
    int optimstatus;
    double objval;
    int count;
    int error;
    int result;
    error = 0;
    count = 0;
    result = 0;
    env = NULL;
    model = NULL;
    allocate_ilp_variables(&ind, &val, &lb, &vtype, &names, &namestorage, rows_columns);
    create_empty_model(game_board, lb, vtype, names, namestorage, cursor, rows_columns); // creating an empty model
    error = GRBloadenv(&env, "sudoku.log"); // creating an environment
    if (error) goto QUIT;
    error = GRBnewmodel(env, &model, "sudoku", rows_columns * rows_columns * rows_columns, NULL, lb, NULL,
                        vtype, names); // creating a new model
    if (error) goto QUIT;
    error = create_single_value_per_cell_constraints(model, ind, val, error, rows_columns); // Each cell gets a value
    if (error) goto QUIT;
    error = create_appear_once_in_row_constrains(game_board, model, ind, val, error, rows_columns); // Each value must appear once in each row
    if (error) goto QUIT;

    // Each value must appear once in each column
    error = create_appear_once_in_column_constrains(model, ind, val, error, rows_columns);
    if (error) goto QUIT;

    // Each value must appear once in each block
    error = create_appear_once_in_block_constrains(model, ind, val, count, error, rows_columns, rows_per_block, columns_per_block);
    if (error) goto QUIT;

    error = GRBoptimize(model); // optimizing model
    if (error) goto QUIT;

    error = GRBwrite(model, "sudoku.lp"); // TODO: check what should i do with the solution
    if (error) goto QUIT;

    // Capture solution information
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval); // TODO: check if i need to change this print message
        result = 1; // success
    } else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n"); // TODO: check if i need to change this print message
        result = 0;
    } else {
        printf("Optimization was stopped early\n"); // TODO: check if i need to change this print message
        result = 0;
    }
    printf("\n");

    QUIT:
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env)); // reporting the error
        return 0;
    }
    free_ilp_variables(ind, val, lb, vtype, names, namestorage);
    GRBfreemodel(model); // free model
    GRBfreeenv(env); // free environment
    return result;
}



