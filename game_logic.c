#include <stdio.h>
#include "game_logic.h"

void execute_solve(char *params){
    // parse args
    // call solve();
    GAME_MODE = 2;
    printf("hello solve\n");
}
void execute_edit(char *params){
    GAME_MODE = 1;
    printf("hello edit\n");
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


void solve(char *path[]){
    // should load the board from file
}

void edit(char *path[]){

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