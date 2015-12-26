#include <stdbool.h>

/* Reads line from standard input
 * Returns false when input is empty */
bool readLine();

/* Gets next word from read line, and writes it to argument
 * Returns -1 when line does not contain words */
int getWord(char* word);

/* Gets integer from read line
 * Returns -1 when line does not contain a number
 * In other case, returns that integer */
int getInt(char* word);

/* Returns true if line contains some more symbols */
bool lineFinished();
