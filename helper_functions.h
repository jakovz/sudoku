#ifndef SUDOKU_HELPER_FUNCTIONS_H
#define SUDOKU_HELPER_FUNCTIONS_H

#endif /* SUDOKU_HELPER_FUNCTIONS_H */

/* generic helper functions which are not directly related to the game itself. */


/*
 * splits a string by the given delimiter.
 * input: char *a_str - the given string to split, char a_delim - the delimiter, size_t *count - number of substrings
 * output: char ** array containing the split pieces of the string
 */
char **str_split(char *a_str, char a_delim, size_t *count);
