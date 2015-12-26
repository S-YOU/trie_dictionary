/* Inserts word to dictionary
 * Returns -1 in case of failure */
int insert(char* word);

/* Inserts subword [start:end] of an existing word
 * Returns -1 in case of failure */
int prev(int number, int start, int end);

/* Removes word from dictionary
 * Returns -1 in case of failure */
int delete(int number);

/* Returns 0 if dictionary contains a word
 * -1 in other case */
int find(char* word);

void clear();

// Returns number of nodes
int getNodes();
