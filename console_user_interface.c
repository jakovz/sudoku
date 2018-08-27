#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console_user_interface.h"
#include "game_logic.h"
#include "helper_functions.h"
#include "game_board_operations.h"

const char *all_commands[] = {"solve", "edit", "print_board", "set", "validate", "generate", "undo", "redo",
                              "save", "num_solutions", "autofill", "hint", "mark_errors", "reset", "exit", NULL};


const char *edit_commands[] = {"solve", "edit", "print_board", "set", "validate", "generate", "undo", "redo",
                               "save", "num_solutions", "reset", "exit", NULL};
const char *solve_commands[] = {"solve", "edit", "mark_errors", "print_board", "set", "validate", "undo", "redo",
                                "save", "hint", "num_solutions", "autofill", "reset", "exit", NULL};
const char *init_commands[] = {"solve", "edit", "exit", NULL};


void execute_set_cell(char **params) {
    int set_cell_params[3];
    int i;
    char *next;
    for (i = 0; i < 3; i++) {
        set_cell_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    set_cell_params[0] = set_cell_params[0] - 1;
    set_cell_params[1] = set_cell_params[1] - 1;
    set_cell(set_cell_params[1], set_cell_params[0], set_cell_params[2]);
}

void execute_generate(char **params) {
    int generate_params[2];
    int i;
    char *next;
    for (i = 0; i < 2; i++) {
        generate_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
            return;
        }
    }
    generate(generate_params[0], generate_params[1]);
}

void execute_save_board(char **params) {
    save_board(params[0]);
}

void execute_get_hint(char **params) {
    int get_hint_params[2];
    int i;
    char *next;
    for (i = 0; i < 2; i++) {
        get_hint_params[i] = strtol(params[i], &next, 10);
        if (next <= params[i]) {
            printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
            return;
        }
    }
    get_hint(get_hint_params[0], get_hint_params[1]);
}

void execute_mark_errors(char **params) {
    int mark_errors_param;
    char *next;
    mark_errors_param = strtol(params[0], &next, 10);
    if (next <= params[0] || mark_errors_param > 1 || mark_errors_param < 0) {
        printf("Error: the value should be 0 or 1\n");
        return;
    }
    mark_errors(mark_errors_param);
}


int execute_command(char *command, char **params) {
    /* gets the command as a string and invokes it's corresponding function */
    if (strcmp(all_commands[0], command) == 0) {
        if (params == NULL) {
            printf("Error: invalid command\n");
            return 1;
        }
        solve(params[0]);
    } else if (strcmp(all_commands[1], command) == 0) {
        if (params == NULL) {
            edit(NULL);
        } else {
            edit(params[0]);
        }
    } else if (strcmp(all_commands[2], command) == 0) {
        print_board(game_board);
    } else if ((strcmp(all_commands[3], command) == 0)) {
        execute_set_cell(params);
    } else if ((strcmp(all_commands[4], command) == 0)) {
        validate_solution();
    } else if ((strcmp(all_commands[5], command) == 0)) {
        execute_generate(params);
    } else if ((strcmp(all_commands[6], command) == 0)) {
        undo(1);
    } else if ((strcmp(all_commands[7], command) == 0)) {
        redo();
    } else if ((strcmp(all_commands[8], command) == 0)) {
        execute_save_board(params);
    } else if ((strcmp(all_commands[9], command) == 0)) {
        num_solutions();
    } else if ((strcmp(all_commands[10], command) == 0)) {
        autofill();
    } else if ((strcmp(all_commands[11], command) == 0)) {
        execute_get_hint(params);
    } else if ((strcmp(all_commands[12], command) == 0)) {
        execute_mark_errors(params);
    } else if (strcmp(all_commands[13], command) == 0) {
        restart_game();
    } else if ((strcmp(all_commands[14], command) == 0)) {
        return 0;
    } else {
        printf("Error: invalid command\n");
    }
    return 1;
}


int check_if_suitable_command(const char *commands[], char *command) {
    int i;
    i = 0;
    while (commands[i] != NULL) {
        if (strcmp(command, commands[i]) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

void play_game() {
    char *command_and_parameters;
    char *command;
    char *parameters;
    char **splitted_params;
    int command_max_length;
    size_t count;
    int i;
    count = 0;
    printf("Sudoku\n------\n");
    command_max_length = COMMAND_AND_PARAMS_SIZE;
    command_and_parameters = (char *) malloc(sizeof(char) * command_max_length);
    if (!command_and_parameters) {
        printf("Error: play_game() has failed\n");
        exit(-1);
    }
    printf("Enter your command:\n");
    while (fgets(command_and_parameters, command_max_length, stdin) != NULL) {
        if (strlen(command_and_parameters) == COMMAND_AND_PARAMS_SIZE) {
            /* means that the command is too long the therefore should be considered as invalid. */
            printf("ERROR: invalid command\n");
        }
        command = strtok(command_and_parameters, " \t\r\n");
        parameters = strtok(NULL, "\t\r\n");
        if (command == NULL) {
            /*  we ignore any kind of blank characters */
            continue;
        }
        if (parameters != NULL) {
            splitted_params = str_split(parameters, ' ');
        } else {
            splitted_params = NULL;
        }
        /*  checking if the command is allowed in the current mode */
        if (GAME_MODE == 0) {
            if (!check_if_suitable_command(init_commands, command)) {
                printf("ERROR: invalid command\n");
                continue;
            }
        } else if (GAME_MODE == 1) {
            if (!check_if_suitable_command(edit_commands, command)) {
                printf("ERROR: invalid command\n");
                continue;
            }
        } else if (GAME_MODE == 2) {
            if (!check_if_suitable_command(solve_commands, command)) {
                printf("ERROR: invalid command\n");
                continue;
            }
        }
        if (!execute_command(command, splitted_params)) {
            /* means that an exit command was given */
            break;
        }
        print_board(game_board);
        printf("Enter your command:\n");
    }
    for (i = 0; i < (int) count; i++) {
        free(splitted_params[i]);
    }
    free(splitted_params);
    free(command_and_parameters);
    exit_game();
}
