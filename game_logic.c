#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <mem.h>
#include "game_logic.h"


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
    // should load the board from file
    FILE *fp;
    int i;
    int j;
    int c;
    int ROWS_PER_BLOCK;
    int COLUMNS_PER_BLOCK;
    GAME_MODE = 2;
    fp = fopen(path,"r");
    if (fp == NULL){
        printf("Error: File doesn't exist or cannot be opened\n");
        return;
    }
    else{
        fscanf(fp, " "); /*skip whitespaces*/
        ROWS_PER_BLOCK = fgetc(fp);
        fscanf(fp, " "); /*skip whitespaces*/
        //fgetc(fp); /*space*/
        COLUMNS_PER_BLOCK = fgetc(fp);
        fscanf(fp, " "); /*skip whitespaces*/
        //fgetc(fp); /*\n*/
        for (i = 0; i< ROWS_PER_BLOCK*COLUMNS_PER_BLOCK; i++){
            for (j = 0; j< ROWS_PER_BLOCK*COLUMNS_PER_BLOCK; j++){
                game_board[i][j] = fgetc(fp);
                if (feof(fp)){
                    break;
                } else if ((c = fgetc(fp)) == '.'){
                    fixed_numbers_board[i][j] = 1;
                    fscanf(fp, " "); /*skip whitespaces*/
                    //fgetc(fp); /*space or \n*/
                } else{
                    fscanf(fp, " "); /*skip whitespaces*/
                    //fgetc(fp); /*space or \n*/
                }
            }
        }
    }
    fclose(fp);
}

void edit(char *path){

}

void mark_errors(int X){

}

void set_cell(int x, int y, int z){

}

void undo(){

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
    printf("Sudoku\n------\n");
    GAME_MODE = 1;
    ROWS_PER_BLOCK = 3;
    COLUMNS_PER_BLOCK = 3;
    ROWS_COLUMNS_NUM = ROWS_PER_BLOCK*COLUMNS_PER_BLOCK;
    EMPTY_CELLS_NUM = ROWS_COLUMNS_NUM*ROWS_COLUMNS_NUM;
    game_board = malloc(sizeof(int)*9);
    erroneous_board = malloc(sizeof(int)*9);
    fixed_numbers_board = malloc(sizeof(int)*9);
    for (i=0; i<9;i++){
        game_board[i] = malloc(sizeof(int)*9);
        fixed_numbers_board[i] = malloc(sizeof(int)*9);
        erroneous_board[i] = malloc(sizeof(int)*9);
    }
    for(i=0;i<9; i++){
        for(j=0; j<9; j++) {
            game_board[i][j]= 3;
            erroneous_board[i][j]= 1;
        }
    }
}