#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <mem.h>
#include "game_logic.h"
#include "helper_functions.h"


void execute_solve(char **params){
    // parse args
    // call solve();
    solve(params[0]);
}
void execute_edit(char *params){
}
void execute_set_cell(char *params){
    printf("hello set_cell\n");
}
void execute_generate(char *params){
    printf("hello generate\n");
}
void execute_save_board(char *params){
    printf("hello save_board\n");
}
void execute_get_hint(char *params){
    printf("hello get_hint\n");
}
void execute_mark_errors(char *params){
    printf("hello mark_errors\n");
}


void solve(char *path){
    // should load the board from file
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

void save_board(char *path[]){

}

void get_hint(int x, int y){

}

void validate_solution(){
    // a call to ilp solver here
}

void generate(int parameters, int i){

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