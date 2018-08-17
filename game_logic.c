#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <mem.h>
#include "game_logic.h"
#include "helper_functions.h"


void execute_set_cell(char **params){
    int set_cell_params[3];
    int i;
    char *next;
    for (i=0;i<3;i++){
        set_cell_params[i] = strtol(params[i], &next, 10);
        if (next<=params[i]){
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    set_cell(set_cell_params[0],set_cell_params[1],set_cell_params[2]);
}
void execute_generate(char **params){
    int generate_params[2];
    int i;
    char *next;
    for (i=0;i<2;i++){
        generate_params[i] = strtol(params[i], &next, 10);
        if (next<=params[i]){
            printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
            return;
        }
    }
    generate(generate_params[0], generate_params[1]);
}

void execute_save_board(char **params){
    save_board(params[0]);
    printf("hello save_board\n");
}

void execute_get_hint(char **params){
    int get_hint_params[2];
    int i;
    char *next;
    for (i=0;i<2;i++){
        get_hint_params[i] = strtol(params[i], &next, 10);
        if (next<=params[i]){
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    get_hint(get_hint_params[0], get_hint_params[1]);
}

void execute_mark_errors(char **params){
    int mark_errors_param;
    char *next;
    mark_errors_param = strtol(params[0], &next, 10);
    if (next<=params[0] || mark_errors_param>1){
        printf("Error: the value should be 0 or 1\n");
        return;
    }
}

void solve(char *path){
    // loads the board from file
    FILE *fp;
    int i;
    int j;
    int c;
    GAME_MODE = 2;
    fp = fopen(path,"r");
    if (fp == NULL){
        printf("Error: File doesn't exist or cannot be opened\n");
        return;
    }
    else{
        fscanf(fp, " "); /*skip whitespaces*/
        fscanf(fp, "%d", &ROWS_PER_BLOCK);
        fscanf(fp, " "); /*skip whitespaces*/
        fscanf(fp, "%d", &COLUMNS_PER_BLOCK);
        init_game();
        fscanf(fp, " "); /*skip whitespaces*/
        for (i = 0; i< ROWS_COLUMNS_NUM; i++){
            for (j = 0; j< ROWS_COLUMNS_NUM; j++){
                 fscanf(fp, "%d", &(game_board[i][j]));
                if (feof(fp)){
                    break;
                } else if ((c = fgetc(fp)) == '.'){
                    fixed_numbers_board[i][j] = 1;
                    fscanf(fp, " "); /*skip whitespaces*/
                } else{
                    fscanf(fp, " "); /*skip whitespaces*/
                }
            }
        }
    }
    fclose(fp);
}

void edit(char *path){

}

void mark_errors(int X){
    MARK_ERRORS = X;
}

void set_cell(int x, int y, int z){
    int old;
    old = game_board[x][y];
    game_board[x][y] = z;
    update_moves_list(x,y,z, old);
}
void update_moves_list(int x, int y, int z, int old) {
    struct game_move *last_move;
    last_move = game_moves;
    (*game_moves).next = malloc(sizeof(struct game_move));
    game_moves = (*game_moves).next;
    (*game_moves).x_value = x;
    (*game_moves).y_value = y;
    (*game_moves).new_z_value = z;
    (*game_moves).old_z_value = old;
    (*game_moves).prev = last_move;
}


void undo(){
    game_board[(*game_moves).x_value][(*game_moves).y_value] = (*game_moves).old_z_value;
    game_moves = (*game_moves).prev;
}

void redo(){

}

void save_board(char *path){

}

void get_hint(int x, int y){

}

void validate_solution(){
    // a call to ilp solver here
}

void generate(int x, int y){

}

void num_solutions(){

}

void autofill(){

}

void restart_game(){

}

void exit_game(){

}

void init_game(){
    int i;
    int j;
    ROWS_COLUMNS_NUM = ROWS_PER_BLOCK*COLUMNS_PER_BLOCK;
    EMPTY_CELLS_NUM = ROWS_COLUMNS_NUM*ROWS_COLUMNS_NUM;
    game_board = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    erroneous_board = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    fixed_numbers_board = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    for (i=0; i<ROWS_COLUMNS_NUM;i++){
        game_board[i] = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
        fixed_numbers_board[i] = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
        erroneous_board[i] = malloc(sizeof(int)*ROWS_COLUMNS_NUM);
    }
}