#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "helper_functions.h"

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
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
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            if (idx >= count){
                printf("Error: parsing parameters 1 failed\n");
                exit(-1);
            }
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        if (idx != count - 1){
            printf("Error: parsing parameters 2 failed\n");
            exit(-1);
        }
        *(result + idx) = 0;
    }

    return result;
}