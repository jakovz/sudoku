#include <stdio.h>


char **get_and_parse_command(){
    // takes care only of parsing the command from user input.
    // i.e validating it's length, ignoring whitespaces, \n's etc.
}

void init(){
    // responsible for handling the init phase
}

void edit(){
    // responsible for handling the edit phase
}

void solve(){
    // responsible for handling the solve phase
}

void play_game(){
    // all game cycle

    // first init
    init();

}

void print_board(){

}

void print_error(const char *error[]){
    printf("Error: %s\n", (char *)error);
}