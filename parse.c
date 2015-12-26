/* Simple parser implementation
 * author : Marcin Gregorczyk */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

#define BUFFER_SIZE 100009

static char buffer[BUFFER_SIZE];
static int eol;  // end of line
static int buffer_ptr;


bool readLine() {
  eol = 0, buffer_ptr = 0;
 
  while((buffer[eol] = getchar()) != '\n' && buffer[eol] != EOF)
    eol++;
  
  if(buffer[0] == EOF)
    return false;

  return true;
}


static void getNext(char* word, bool integer) {
  int end;

  while(buffer_ptr < eol && buffer[buffer_ptr] == ' ')
    buffer_ptr++;

  end = buffer_ptr;

  if(integer)
    while(end < eol && buffer[end] >= '0' && buffer[end] <= '9') 
      end++;
  else
    while(end < eol && buffer[end] >= 'a' && buffer[end] <= 'z')
      end++;

  if(buffer_ptr == end) {
    word[0] = '\0';
  }
  else {
    strncpy(word, buffer + buffer_ptr, end - buffer_ptr);
    word[end-buffer_ptr] = '\0';
    buffer_ptr = end;
  }
}


bool lineFinished() {
  for(int i = buffer_ptr; i < eol; i++)
    if(buffer[i] != ' ')
      return false;

  return true;
}

int getWord(char* word) {
  getNext(word, false);
  if(strcmp(word, "") == 0)
    return -1;

  return 0;
}

int getInt(char* word) {
  getNext(word, true);
  if(strcmp(word, "") == 0 || strlen(word) > 8)
    return -1;
  if(word[0] == '0' && atoi(word) != 0)
    return -1;
  
  return atoi(word);
}
