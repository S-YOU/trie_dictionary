/* Simple implementation of prefix dictionary
 * author : Marcin Gregorczyk */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "trie.h"

#define MAX_WORD_SIZE 100009

char word[MAX_WORD_SIZE];
bool debugMode = false;

void ignore() {
  printf("ignored\n");
}


void printNodes() {
  fprintf(stderr, "nodes: %d\n", getNodes());
}


void performInsert() {
  if(getWord(word) == -1 || !lineFinished()) {
    ignore();
  }
  else {
    int result = insert(word);
    if(result == -1) {
      ignore();
    }
    else {
      printf("word number: %d\n", result);
      if(debugMode) printNodes();
    } 
  }
}


void performPrev() {
  int n, b, e;

  n = getInt(word);
  b = getInt(word);
  e = getInt(word);

  if(n == -1 || b == -1 || e == -1 || !lineFinished()) {
    ignore();
  }
  else {
    int result = prev(n, b, e);
    if(result == -1) {
      ignore();
    }
    else {
      printf("word number: %d\n", result);
      if(debugMode) printNodes();
    }
  }
}


void performDelete() {
  int n = getInt(word);

  if(n == -1 || !lineFinished()) {
    ignore();
  }
  else {
    int result = delete(n);
    if(result == -1) {
      ignore();
    }
    else {
      printf("deleted: %d\n", result);
      if(debugMode) printNodes();
    }
  }
}


void performFind() {
  if(getWord(word) == -1 || !lineFinished())
    ignore();
  else if(find(word) == 0)
    printf("YES\n");
  else
    printf("NO\n");
}

void performClear() {
  if(!lineFinished()) {
    ignore();
  }
  else {
    clear();
    printf("cleared\n");
    if(debugMode) printNodes();
  }
}

int main(int argc, char** argv) {
  for(int i = 1; i < argc; i++) {
    if(strcmp(argv[i], "-v") == 0) {
      debugMode = true;
    }
    else {
      printf("Incorrect parameters\n");
      return 0;
    }
  }

  while(readLine()) {
    if(getWord(word) == -1)
      ignore();
    else if(strcmp(word, "insert") == 0)
      performInsert();
    else if(strcmp(word, "prev") == 0)
      performPrev();
    else if(strcmp(word, "delete") == 0)
      performDelete();
    else if(strcmp(word, "find") == 0)
      performFind();
    else if(strcmp(word, "clear") == 0)
      performClear();
    else ignore();
  }

  clear();
  return 0;
}
