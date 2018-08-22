#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include "exhaustive_backtracking_solver.h"
#include "helper_functions.h"

/*TODO: credit to https://www.geeksforgeeks.org/stack-data-structure-introduction-program/*/
struct StackNode* newNode(int rows_index, int columns_index, int **new_solved_board, int current) {
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    (*stackNode).rows_index = rows_index;
    (*stackNode).columns_index = columns_index;
    (*stackNode).(**new_solved_board) = new_solved_board;
    (*stackNode).current = current;
    (*stackNode).next = NULL;
    return stackNode;
}
int isEmpty(struct StackNode *root) {
    return !root;
}
void push(struct StackNode** root, int rows_index, int columns_index, int **new_solved_board, int current) {
    struct StackNode* stackNode = newNode(rows_index, columns_index, new_solved_board, current);
    (*stackNode).next = *root;
    *root = stackNode;
}
struct StackNode* pop(struct StackNode** root) {
    //TODO: check if(isEmpty(*root)) before every call to pop()
    struct StackNode* temp = *root;
    *root = (*(*root)).next;
    free(temp); //TODO: free only afterr we call this func
    return temp;
}
int exhaustive_backtracking(int rows_index, int columns_index, int **new_solved_board, int current, int rows_columns_num,
                        int rows_per_block, int columns_per_block) {
    /*returns num of possible solutions to a given board (if the current board is solvable) using an exhaustive (deterministic) backtracking algorithm*/
    int *available_numbers;
    int num_of_solutions;
    int counter;
    int i;
    int j;
    int p;
    struct StackNode* root = NULL;
    struct StackNode* tmp = NULL;
    counter = 0;
    num_of_solutions = 0;
    available_numbers = (int *)malloc(sizeof(int)*rows_columns_num);
    while (new_solved_board[rows_index][columns_index] > 0) {
        if ((columns_index + 1) < rows_columns_num) {
            columns_index++;
        } else if ((rows_index + 1) < rows_columns_num) {
            columns_index = 0;
            rows_index++;
        }
            /* if this is the last cell and its value is already set*/
        else if (((rows_index + 1) == rows_columns_num) && ((columns_index + 1) == rows_columns_num)) {
            num_of_solutions++;
            return num_of_solutions;
        }
    }
    for (i=0; i<rows_columns_num; i++){
        available_numbers[i] = 0;
    }
    get_available_numbers_for_set(available_numbers, rows_index, columns_index, new_solved_board, rows_columns_num, rows_per_block, columns_per_block);
    /* creating the actual available numbers array*/
    for (i = 0; i < rows_columns_num; i++) {
        if (available_numbers[i] == 0 && (i + 1) > current) {
            counter++;
        }
    }
    /* backtracking from the first emptyy cell - means that the board is unsolvable, and there is no valid solution as is.*/
    if (counter == 0) {
        return num_of_solutions;
    }
    for (p = 0; p < rows_columns_num; p++) {
        if (available_numbers[p] == 0 && (p + 1) > current) {
            new_solved_board[rows_index][columns_index] = p + 1; //TODO:remove? cell should still be 0 (to find another solution)
            current = p + 1;
            break;
        }
    }
    push(&root, rows_index, columns_index, new_solved_board, current);
    if ((columns_index + 1) < rows_columns_num){
        push(&root, rows_index, columns_index + 1, new_solved_board, 0);
    } else if ((rows_index + 1) < rows_columns_num){
        push(&root, rows_index + 1, 0, new_solved_board, 0);
    }
    while(!isEmpty(root)){
        tmp = pop(&root);
        rows_index = (*tmp).rows_index;
        columns_index = (*tmp).columns_index;
        current = (*tmp).current;
        if(current_indicators_board[rows_index][columns_index] == 0){
            new_solved_board[rows_index][columns_index] = 0;
        }
        while (new_solved_board[rows_index][columns_index] > 0) {
            if ((columns_index + 1) < rows_columns_num) {
                columns_index++;
            } else if ((rows_index + 1) < rows_columns_num) {
                columns_index = 0;
                rows_index++;
            }
            /*else if this is the last cell and its value is already set - increment value of last cell ||OR|| backtrack if can't increment*/
            else if (((rows_index + 1) == rows_columns_num) && ((columns_index + 1) == rows_columns_num)) {
                num_of_solutions++;
                tmp = pop(&root);
                rows_index = (*tmp).rows_index;
                columns_index = (*tmp).columns_index;
                current = (*tmp).current;
                //break out of whileloop in order to increment this cell || backtrack
            }
        }
        for (i=0; i<rows_columns_num; i++){
            available_numbers[i] = 0;
        }
        get_available_numbers_for_set(available_numbers, rows_index, columns_index, new_solved_board, rows_columns_num, rows_per_block, columns_per_block);
        /* creating the actual available numbers array*/
        for (i = 0; i < rows_columns_num; i++) {
            if (available_numbers[i] == 0 && (i + 1) > current) {
                counter++;
            }
        }
        if (counter == 0) {
            //new_solved_board[rows_index][columns_index] = 0; //TODO:need this?
            continue;
        }
        for (p = 0; p < rows_columns_num; p++) {
            if (available_numbers[p] == 0 && (p + 1) > current) {
                new_solved_board[rows_index][columns_index] = p + 1;
                current = p + 1;
                break;
            }
        }
        push(&root, rows_index, columns_index, new_solved_board, current);
        if ((columns_index + 1) < rows_columns_num){
            push(&root, rows_index, columns_index + 1, new_solved_board, 0);
        } else if ((rows_index + 1) < rows_columns_num){
            push(&root, rows_index + 1, 0, new_solved_board, 0);
        }
    }
    return num_of_solutions;
}