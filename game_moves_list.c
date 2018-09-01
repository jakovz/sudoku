#include <stdio.h>
#include <stdlib.h>
#include "game_moves_list.h"

void initialize_game_moves_list() {
    if (game_moves == NULL) {
        /* initializing moves list */
        game_moves = (struct game_move *) malloc(sizeof(struct game_move));
        (*game_moves).prev = NULL;
        (*game_moves).next = NULL;
        (*game_moves).y_value = 0;
        (*game_moves).x_value = 0;
        (*game_moves).new_z_value = 0;
        (*game_moves).old_z_value = 0;
    }
}

void update_moves_list(int x, int y, int z, int old, int autofill_generate_value) {
    struct game_move *last_move;
    initialize_game_moves_list();
    last_move = game_moves;
    free_next_moves();
    (*game_moves).next = (struct game_move *) malloc(sizeof(struct game_move));
    if ((*game_moves).next == NULL){
        printf("Error: could not set the move\n");
        exit(-1);
    }
    game_moves = (*game_moves).next;
    (*game_moves).x_value = x;
    (*game_moves).y_value = y;
    (*game_moves).new_z_value = z;
    (*game_moves).old_z_value = old;
    (*game_moves).generate_autofill_command = autofill_generate_value;
    (*game_moves).prev = last_move;
    (*game_moves).next = NULL;
}

void clear_moves_list() {
    struct game_move *current;
    while ((*game_moves).prev != NULL) {
        game_moves = (*game_moves).prev;
    }
    /* now we are on the first (sentinel) move */
    if ((*game_moves).next == NULL) {
        free(game_moves);
        game_moves = NULL;
        return;
    }
    while ((*game_moves).next != NULL) {
        current = game_moves;
        game_moves = (*game_moves).next;
        free(current);
    }
    if ((*game_moves).next == NULL) {
        free(game_moves);
        game_moves = NULL;
        return;
    }
}

void free_next_moves() {
    struct game_move *current;
    struct game_move *head;
    current = game_moves;
    head = game_moves;
    while ((*current).next != NULL) {
        current = (*current).next;
        free(head);
        head = current;
    }
}

