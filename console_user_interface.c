#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "game_logic.h"
#include "console_user_interface.h"


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
    char *command_and_parameters;
    char *command;
    char *parameters;
    char *exit_string;
    char *set;
    char *hint;
    char *validate;
    char *restart;
    int command_max_length;
    command_max_length = COMMAND_AND_PARAMS_SIZE;
    exit_string = "exit";
    set = "set";
    hint = "hint";
    validate = "validate";
    restart = "restart";
    command_and_parameters = (char *) malloc(sizeof(char) * command_max_length);
    if (!command_and_parameters) {
        printf("Error: play_game() has failed\n");
        exit(-1);
    }
    fflush(stdin);
    while (fgets(command_and_parameters, command_max_length, stdin) != NULL) {
        command = strtok(command_and_parameters, " \t\r\n");
        parameters = strtok(NULL, "\t\r\n");
        fflush(stdin);
        if (command==NULL){
            fflush(stdin);
            continue;
        }
        else if (strcmp(set, command) == 0 && IS_GAME_BOARD_SOLVED == 0) {
            set_cell(parameters);
        } else if (strcmp(hint, command) == 0 && IS_GAME_BOARD_SOLVED == 0) {
            get_hint(parameters);
        } else if (strcmp(validate, command) == 0 && IS_GAME_BOARD_SOLVED == 0) {
            validate_solution();
        } else if (strcmp(restart, command) == 0) {
            restart_game();
        } else if ((strcmp(exit_string, command) == 0)){
            break;
        } else {
            printf("Error: invalid command\n");
        }
        fflush(stdin);
    }
    free(command_and_parameters);
    printf("Exiting...\n");
}

void print_board(int board[ROWS_NUM][COLUMNS_NUM]){
    int i;
    int j;
    int z;
    int rows_counter;
    int columns_counter;
    rows_counter = 0;
    for (i = 0; i < ROWS_NUM + 4; i++) {
        if ((i % 4) == 0) {
            for (z = 0; z < (COLUMNS_NUM / 3) * 11; z++) {
                printf("-");
            }
            printf("-\n");
            rows_counter++;
        } else {
            columns_counter = 0;
            for (j = 0; j < COLUMNS_NUM + 4; j++) {
                if ((j % 4) == 0) {
                    if (j == 0) {
                        printf("|");
                    } else if (j == COLUMNS_NUM + 3) {
                        printf(" |\n");
                    } else {
                        printf(" |");
                    }
                    columns_counter++;
                } else {
                    if (fixed_numbers_board[i - rows_counter][j - columns_counter] == 1) {
                        /*means that this is a fixed number*/
                        printf(" .");
                    } else {
                        printf("  ");
                    }
                    if (board[i - rows_counter][j - columns_counter] == 0) {
                        printf(" ");
                    } else {
                        printf("%d", board[i - rows_counter][j - columns_counter]);
                    }
                }
            }
        }
    }
}

void print_error(const char *error[]){
    printf("Error: %s\n", (char *)error);
}