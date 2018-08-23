#include <stdio.h>
#include <stdlib.h>
#include "exhaustive_backtracking_solver.h"
#include "helper_functions.h"

/*TODO: credit to https://www.geeksforgeeks.org/stack-data-structure-introduction-program/*/
struct StackNode* newNode(int rows_index, int columns_index, int current) {
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    (*stackNode).rows_index = rows_index;
    (*stackNode).columns_index = columns_index;
    (*stackNode).current = current;
    (*stackNode).next = NULL;
    return stackNode;
}
int isEmpty(struct StackNode *root) {
    return !root;
}
void push(struct StackNode** root, int rows_index, int columns_index, int current) {
    struct StackNode* stackNode = newNode(rows_index, columns_index, current);
    (*stackNode).next = *root;
    *root = stackNode;
}
struct StackNode pop(struct StackNode** root) {
    struct StackNode tmp;
    struct StackNode *second_tmp;
    second_tmp = *root;
    tmp.columns_index = (*(*root)).columns_index;
    tmp.current = (*(*root)).current;
    tmp.rows_index = (*(*root)).rows_index;
    *root = (*(*root)).next;
    free(second_tmp);
    return tmp;
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
    struct StackNode* root;
    struct StackNode tmp;
    root = NULL;
    counter = 0;
    num_of_solutions = 0;
    available_numbers = (int *)malloc(sizeof(int)*rows_columns_num);
    if (available_numbers==NULL){
        printf("Error: Exhaustive backtracking failed\n");
        return -1;
    }
    printf("allocated available numbers");

    while((!isEmpty(root)) || (rows_index==0 && columns_index==0)){
        while((rows_index < rows_columns_num) && (columns_index < rows_columns_num)){
            if(current_indicators_board[rows_index][columns_index] == 0){
                if(new_solved_board[rows_index][columns_index] == 0){
                    current = 0;
                }
                for (i=0; i<rows_columns_num; i++){
                    available_numbers[i] = 0;
                }
                printf("after available numbers for loop\n");
                get_available_numbers_for_set(available_numbers, rows_index, columns_index, new_solved_board, rows_columns_num, rows_per_block, columns_per_block);
                printf("after get available numbers for set\n");
                /* creating the actual available numbers array*/
                for (i = 0; i < rows_columns_num; i++) {
                    if (available_numbers[i] == 0 && (i + 1) > current) {
                        counter++;
                    }
                }
                /* backtracking from the first emptyy cell - means that the board is unsolvable, and there is no valid solution as is.*/
                if (counter == 0) {
                    tmp = pop(&root);
                    rows_index = tmp.rows_index;
                    columns_index = tmp.columns_index;
                    current = tmp.current;
                    /*initializing every cell after the one we just popped*/
                    for (i=rows_index+1; i<rows_columns_num; i++){
                        for (j=columns_index+1; j<rows_columns_num;j++){
                            if(current_indicators_board[i][j] == 0){
                                new_solved_board[i][j] = 0;
                            }
                        }
                    }
                    continue;
                }
                for (p = 0; p < rows_columns_num; p++) {
                    if (available_numbers[p] == 0 && (p + 1) > current) {
                        new_solved_board[rows_index][columns_index] = p + 1;
                        current = p + 1;
                        break;
                    }
                }
                push(&root, rows_index, columns_index, current);
            }
            if ((columns_index + 1) < rows_columns_num) {
                columns_index++;
            } else if ((rows_index + 1) < rows_columns_num) {
                columns_index = 0;
                rows_index++;
            } /*else if this is the last cell and its value is already set - increment value of last cell ||OR|| backtrack if can't increment*/
            else if (((rows_index + 1) == rows_columns_num) && ((columns_index + 1) == rows_columns_num)) {
                num_of_solutions++;
                tmp = pop(&root);
                rows_index = tmp.rows_index;
                columns_index = tmp.columns_index;
                current = tmp.current;
            }
        }
        //num_of_solutions++;
    }
    free(available_numbers);
    return num_of_solutions;
}