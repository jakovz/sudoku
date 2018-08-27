#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "helper_functions.h"

/* Credit: https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c */
char **str_split(char *a_str, const char a_delim, size_t *count) {
    size_t size;
    size_t idx;
    char *token;
    char *p;
    char **result = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    *count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    (*count)++;

    result = malloc(sizeof(char *) * (*count));
    if (result == NULL){
        printf("Error: str_split failed\n");
        exit(-1);
    }
    idx = 0;
    token = strtok(a_str, delim);

    while (token) {
        if (idx >= (*count)){
            printf("Error: parsing parameters failed\n");
            exit(-1);
        }
        size = strlen(token) + 1;
        p = malloc(size);
        if (p) {
            memcpy(p, token, size);
        }
        *(result + idx++) = p;
        token = strtok(0, delim);
        free(p);
    }
    if (idx != (*count) - 1){
        printf("Error: parsing parameters failed\n");
        exit(-1);
    }
    *(result + idx) = 0;
    return result;
}
