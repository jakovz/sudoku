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

/* validates 3 parameters were given for the set command that they are of type int, and calls set_cell
 * input:
 * char **params - array of parameters to the function
 * output: none
 * */
static void execute_set_cell(char **params) {
    int set_cell_params[3];
    int i;
    char *next;
    if (params == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    for (i = 0; i < 3; i++) {
        if (params[i] != NULL) {
            set_cell_params[i] = strtol(params[i], &next, 10);
            if ((*next) != '\0') {
                /* means one of the three first parameters is not an int */
                printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
                return;
            }
        } else {
            printf("ERROR: invalid command\n");
            return;
        }
    }
    set_cell_params[0] = set_cell_params[0] - 1;
    set_cell_params[1] = set_cell_params[1] - 1;
    set_cell(set_cell_params[1], set_cell_params[0], set_cell_params[2]);
}

/* validates 2 parameters were given for the generate command that they are of type int, and calls generate
 * input:
 * char **params - array of parameters to the function
 * output: none
 * */
static void execute_generate(char **params) {
    int generate_params[2];
    int i;
    char *next;
    if (params == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    for (i = 0; i < 2; i++) {
        if (params[i] != NULL) {
            generate_params[i] = strtol(params[i], &next, 10);
            if ((*next) != '\0') {
                printf("Error: value not in range 0-%d\n", EMPTY_CELLS_NUM);
                return;
            }
        } else {
            printf("ERROR: invalid command\n");
            return;
        }

    }
    generate(generate_params[0], generate_params[1]);
}

/* validates that one parameter was given for the save command, and calls save_board
 * input:
 * char **params - array of parameters to the function
 * output: none
 * */
static void execute_save_board(char **params) {
    if (params == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    if (params[0] != NULL) {
        save_board(params[0]);
    } else {
        printf("ERROR: invalid command\n");
        return;
    }
}

/* validates 2 parameters were given for the set command that they are of type int, and calls get_hint
 * input:
 * char **params - array of parameters to the function
 * output: none
 * */
static void execute_get_hint(char **params) {
    int get_hint_params[2];
    int i;
    char *next;
    if (params == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    for (i = 0; i < 2; i++) {
        if (params[i] != NULL) {
            get_hint_params[i] = strtol(params[i], &next, 10);
            if ((*next) != '\0') {
                printf("Error: value not in range 0-%d\n", ROWS_COLUMNS_NUM);
                return;
            }
        } else {
            printf("ERROR: invalid command\n");
            return;
        }
    }
    get_hint(get_hint_params[0], get_hint_params[1]);
}

/* validates 1 parameter was given for the mark_errors command that it is of type int and it is 0 or 1, and calls mark_errors
 * input:
 * char **params - array of parameters to the function
 * output: none
 * */
static void execute_mark_errors(char **params) {
    int mark_errors_param;
    char *next;
    if (params == NULL) {
        printf("ERROR: invalid command\n");
        return;
    }
    if (params[0] != NULL) {
        mark_errors_param = strtol(params[0], &next, 10);
    } else {
        printf("ERROR: invalid command\n");
        return;
    }
    if ((*next) != '\0' || mark_errors_param > 1 || mark_errors_param < 0) {
        printf("Error: the value should be 0 or 1\n");
        return;
    }
    mark_errors(mark_errors_param);
}

/* executes the given command by calling its executor function (or directly the function if only little validation of the parameters is needed)
 * input:
 * char **params - array of parameters to the function
 * char *command - the command to be executed
 * output: none
 * */
static int execute_command(char *command, char **params) {
    /* gets the command as a string and invokes it's corresponding function */
    if (strcmp(all_commands[0], command) == 0) {
        if (params == NULL || params[0] == NULL) {
            printf("ERROR: invalid command\n");
            return 1;
        }
        solve(params[0]);
    } else if (strcmp(all_commands[1], command) == 0) {
        if (params == NULL || params[0] == NULL) {
            edit(NULL);
        } else {
            edit(params[0]);
        }
    } else if (strcmp(all_commands[2], command) == 0) {
        print_board();
    } else if ((strcmp(all_commands[3], command) == 0)) {
        execute_set_cell(params);
    } else if ((strcmp(all_commands[4], command) == 0)) {
        validate_solution(1);
    } else if ((strcmp(all_commands[5], command) == 0)) {
        execute_generate(params);
    } else if ((strcmp(all_commands[6], command) == 0)) {
        undo(1);
    } else if ((strcmp(all_commands[7], command) == 0)) {
        redo(1);
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
        printf("ERROR: invalid command\n");
    }
    return 1;
}

/* frees the array of split paramters that is created by str_split in play_game.
 * input: none
 * char ***splitted_params - a pointer to the array of the parameters
 * size_t count - the number of parameters in splitted_params
 * output: none
 * */
static void free_splitted_params(char ***splitted_params, size_t count) {
    int i;
    if ((*splitted_params) != NULL) {
        for (i = 0; i < (int) count; i++) {
            free((*splitted_params)[i]);
        }
        free(*splitted_params);
    }
    *splitted_params = NULL;
}

/* checks if the given command belongs to the give group of commands
 * input:
 * char *commands[] - the group of commands to be checked
 * char *command - the command to check for
 * output - 1 if the command belongs to the given group and 0 otherwise
 * */
static int check_if_suitable_command(const char *commands[], char *command) {
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

/* controls the game flow - getting commands from the user and calling the corresponding functions
 * input: none
 * output: none
 * */
void play_game() {
    char *command_and_parameters;
    char *command;
    char *parameters;
    char **splitted_params;
    int command_max_length;
    size_t count;
    count = 0;
    splitted_params = NULL;
    MARK_ERRORS = 1;
    printf("Sudoku\n------\n");
    command_max_length = COMMAND_AND_PARAMS_SIZE;
    command_and_parameters = (char *) malloc(sizeof(char) * command_max_length);
    if (command_and_parameters==NULL) {
        printf("Error: play_game() has failed\n");
        exit(-1);
    }
    printf("Enter your command:\n");
    while (fgets(command_and_parameters, command_max_length, stdin) != NULL) {
        count = 0;
        if (strlen(command_and_parameters) == COMMAND_AND_PARAMS_SIZE) {
            /* means that the command is too long the therefore should be considered as invalid. */
            printf("ERROR: invalid command\n");
        }
        command = strtok(command_and_parameters, " \t\r\n");
        parameters = strtok(NULL, "\t\r\n");
        if (command == NULL) {
            /*  we ignore any kind of blank characters */
            printf("Enter your command:\n");
            continue;
        }
        if (parameters != NULL) {
            splitted_params = str_split(parameters, ' ', &count);
        } else {
            splitted_params = NULL;
        }
        /*  checking if the command is allowed in the current mode */
        if (GAME_MODE == 0) {
            if (!check_if_suitable_command(init_commands, command)) {
                printf("ERROR: invalid command\n");
                free_splitted_params(&splitted_params, count);
                printf("Enter your command:\n");
                continue;
            }
        } else if (GAME_MODE == 1) {
            if (!check_if_suitable_command(edit_commands, command)) {
                printf("ERROR: invalid command\n");
                free_splitted_params(&splitted_params, count);
                printf("Enter your command:\n");
                continue;
            }
        } else if (GAME_MODE == 2) {
            if (!check_if_suitable_command(solve_commands, command)) {
                printf("ERROR: invalid command\n");
                free_splitted_params(&splitted_params, count);
                printf("Enter your command:\n");
                continue;
            }
        }
        if (!execute_command(command, splitted_params)) {
            /* means that an exit command was given */
            free_splitted_params(&splitted_params, count);
            break;
        } else {
            free_splitted_params(&splitted_params, count);
        }
        printf("Enter your command:\n");
    }
    if (splitted_params!=NULL) {
        free_splitted_params(&splitted_params, count);
    }
    free(command_and_parameters);
    exit_game();
}
