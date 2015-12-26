/* Simple trie operations implementation
 * author : Marcin Gregorczyk */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "trie.h"

#define BUFFER_SIZE 2100000
#define MAX_WORD_COUNT 500000
#define ALPHABET 26

typedef struct Node_ {
  int begin;
  int end;
  bool isWord;
  struct Node_* next[ALPHABET];
  struct Node_* parent;
} Node;

typedef struct {
  int begin; 
  int length;
  Node* node;
} Word;

/* Buffer for words inserted to dictionary
 * Nodes know only interval of buffer indexes */
static char buffer[BUFFER_SIZE];

static Word words[MAX_WORD_COUNT];

static int buffer_ptr = 0;
static int wordCounter = 0;
static int nodeCounter = 0;

static Node* root = NULL;


static Node* createNode(int begin, int end, bool isWord) {
  Node* result = malloc(sizeof(Node));

  result->begin = begin;
  result->end = end;
  result->isWord = isWord;

  for(int i = 0; i < ALPHABET; i++)
    result->next[i] = NULL;

  nodeCounter++;
  return result;
}


int getNodes() {
  assert(nodeCounter >= 0);
  return nodeCounter;
}


static void connectNodes(Node* child, Node* parent) {
  parent->next[buffer[child->begin] - 'a'] = child;
  child->parent = parent;
}


//Inserts word buffer[begin:end] to node's subtree
static Node* insertToTrie(Node* node, int begin, int end) {
  assert(begin < end);

  int range;
  Node* child = node->next[buffer[begin] - 'a'];

  if(child == NULL) {
    child = createNode(begin, end, true);
    connectNodes(child, node);
    return child;
  }

  range = child->begin;
  while(begin < end && range < child->end && buffer[begin] == buffer[range]) {
    begin++;
    range++;
  }

  if(begin == end) {
    if(range == child->end) {
      if(child->isWord)
        return NULL;

      child->isWord = true;
      return child;
    }

    Node* newNode = createNode(child->begin, range, true);
    child->begin = range;

    connectNodes(child, newNode);
    connectNodes(newNode, node);

    return newNode;
  }
  else if(range == child->end) {
    return insertToTrie(child, begin, end);
  }

  Node* newNode = createNode(child->begin, range, false);
  Node* newWord = createNode(begin, end, true);
  child->begin = range;

  connectNodes(child, newNode);
  connectNodes(newWord, newNode);
  connectNodes(newNode, node);

  return newWord;
}


int insert(char* word) {
  int len = strlen(word);

  words[wordCounter].begin = buffer_ptr;
  words[wordCounter].length = len;

  for(int i = 0; i < len; i++) 
    buffer[buffer_ptr++] = word[i];

  if(root == NULL)
    root = createNode(-1, -1, false);

  words[wordCounter].node = insertToTrie(root, words[wordCounter].begin, buffer_ptr);
  if(words[wordCounter].node == NULL)
    return -1;
  
  return wordCounter++;
}


int prev(int number, int start, int end) {
  int newBegin, newEnd;

  if(number < 0 || number >= wordCounter || words[number].node == NULL)
    return -1;
  if(start > end || end >= words[number].length)
    return -1;

  newBegin = words[number].begin + start;
  newEnd = words[number].begin + end + 1;

  words[wordCounter].begin = newBegin;
  words[wordCounter].length = newEnd - newBegin;

  words[wordCounter].node = insertToTrie(root, newBegin, newEnd);
  if(words[wordCounter].node == NULL)
    return -1;

  return wordCounter++;
}


static int countChildren(Node* node) {
  int result = 0;

  for(int i = 0; i < ALPHABET; i++)
    if(node->next[i] != NULL)
      result++;

  return result;
}


/* Merges node with its child
 * Assumes that node has exactly one child */
static void merge(Node* node) {
  for(int i = 0; i < ALPHABET; i++) {
    if(node->next[i] != NULL) {
      Node* child = node->next[i];

      child->begin -= (node->end - node->begin);
      connectNodes(child, node->parent);

      free(node);
      nodeCounter--;
      return;
    }
  }
}


int delete(int number) {
  if(number < 0 || number >= wordCounter)
    return -1;
  if(words[number].node == NULL)
    return -1;

  Node* current = words[number].node;
  int children = countChildren(current);

  if(children >= 2) {
    current->isWord = false;
  }
  else if(children == 1) {
    current->isWord = false;
    merge(current);
  }
  else if(children == 0) {
    Node* parent = current->parent;
    parent->next[buffer[current->begin] - 'a'] = NULL;

    free(current);
    nodeCounter--;

    int pChildren = countChildren(parent);
    if(pChildren == 1 && parent != root && parent->isWord == false) {
      merge(parent);
    }
    if(pChildren == 0 && parent->isWord == false) {
      // In this case, parent has to be a root
      free(root);
      root = NULL;
      nodeCounter--;
    }
  }

  words[number].node = NULL;
  return number;
}


int find(char* word) {
  Node* node = root;
  if(root == NULL)
    return -1;

  int len = strlen(word);
  int current = 0;
  int currentBuf = 0;

  while(current < len) {
    if(node == root || currentBuf >= node->end) {
      Node* nextNode = node->next[word[current] - 'a'];

      if(nextNode == NULL)
        return -1;

      node = nextNode;
      currentBuf = node->begin;
    }
    if(word[current] != buffer[currentBuf])
      return -1;

    current++;
    currentBuf++;
  }
  return 0;
}


static void clearHelp(Node* v) {
  buffer_ptr = 0;
  wordCounter = 0;
  nodeCounter = 0;

  for(int i = 0; i < ALPHABET; i++) {
    if(v->next[i] != NULL) {
      clearHelp(v->next[i]);
      free(v->next[i]);
    }
  }
}

void clear() {
  if(root != NULL) {
    clearHelp(root);
    free(root);
    root = NULL;
  }
}
