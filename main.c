// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef DEBUG
#define NDEBUG
#endif

#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "tree.h"
#include "utils.h"

// Max size of a VALID input line.
#define MAX_INPUT_LINE_LENGTH (32)

const int MAX_USERS = 65535;
const int MAX_MOVIE_RATING = 2147483647;
const int MAX_K = 2147483647;

// Return values of readInputLine function:
enum input_feedback {
  INPUT_EOF,
  INPUT_IGNORED_LINE,
  INPUT_INVALID,
  INPUT_INVALID_AND_EOF,
  INPUT_OK
};

static void printError() { fprintf(stderr, "ERROR\n"); }

static void addUser(struct Tree tree, int parentUserId, int userId) {
  if (!inRange(0, MAX_USERS, parentUserId) || !inRange(0, MAX_USERS, userId) ||
      !treeAddNode(tree, userId, parentUserId))
    printError();
  else
    printf("OK\n");
}

static void delUser(struct Tree tree, int userId) {
  if (!inRange(0, MAX_USERS, userId) || !treeDelNode(tree, userId))
    printError();
  else
    printf("OK\n");
}

static void addMovie(struct Tree tree, int userId, int movieRating) {
  if (!inRange(0, MAX_USERS, userId) ||
      !inRange(0, MAX_MOVIE_RATING, movieRating) ||
      !treeAddPreference(tree, userId, movieRating))
    printError();
  else
    printf("OK\n");
}

static void delMovie(struct Tree tree, int userId, int movieRating) {
  if (!inRange(0, MAX_USERS, userId) ||
      !inRange(0, MAX_MOVIE_RATING, movieRating) ||
      !treeRemovePreference(tree, userId, movieRating))
    printError();
  else
    printf("OK\n");
}

static void marathon(struct Tree tree, int userId, int k) {
  if (!inRange(0, MAX_USERS, userId) || !inRange(0, MAX_K, k)) {
    printError();
    return;
  }

#ifdef DEBUG
  printf("Marathon on tree:\n");
  printTree(tree);
#endif

  struct List *res = runMarathon(tree, userId, k);

  if (!res) {
    printError();
  } else {
    if (listEmpty(res))
      printf("NONE\n");
    else {
      listPrintContent(res);
      printf("\n");
    }

    listFree(res);
  }
}

static enum input_feedback readInputLine(char *buffer) {
  char c;
  int index_in_buffer = 0;

  switch (c = getchar()) {
  case EOF:
    return INPUT_EOF;

  case '\n':
    return INPUT_IGNORED_LINE;

  // If given line is a comment, just skip to the EOL.
  case '#': {
    while ((c = getchar()) != '\n') {
      if (c == EOF)
        // In the task description in stands, that every proper input line is
        // terminated with '\n', I treat this as an error.
        return INPUT_INVALID_AND_EOF;
    }

    return INPUT_IGNORED_LINE;
  }

  default: {
    // Don't forget about the first character (getchar in switch statement).
    do {
      // Lines must end with '\n' so the input is invalid!
      if (c == EOF)
        return INPUT_INVALID_AND_EOF;

      if (index_in_buffer >= MAX_INPUT_LINE_LENGTH)
        break;

      buffer[index_in_buffer++] = c;
    } while ((c = getchar()) != '\n');

    // [MAX_INPUT_LINE_LENGTH] is large enough to store any vaild input so the
    // inserted line IS NOT valid. NOTE: Trailing zeors are not supported!
    if (c != '\n') {
      // Skip to the end of the line and return an input error.
      while ((c = getchar()) != '\n')
        if (c == EOF)
          return INPUT_INVALID_AND_EOF;

      return INPUT_INVALID;
    }
    buffer[index_in_buffer] = '\0';

    // For tricky case when there is a nullbyte in the middle of the input.
    for (int i = 0; i < index_in_buffer; ++i)
        if (buffer[i] == '\0')
            return INPUT_INVALID;

    return INPUT_OK;
  }
  }
}

int main(void) {
  struct Tree tree = initTree(MAX_USERS + 1);
  enum input_feedback read_line_state = 0;

  // [MAX_INPUT_LINE_LENGTH] characters is more than enought for valid,
  // non-comment input lines. Comment lines are ignored, never stored in buffer.
  static char input_buffer[MAX_INPUT_LINE_LENGTH];

  while ((read_line_state = readInputLine(input_buffer)) != INPUT_EOF) {
    if (read_line_state == INPUT_INVALID) {
      // ERROR: Invalid input.
      printError();
      continue;
    } else if (read_line_state == INPUT_IGNORED_LINE) {
      continue;
    }
    // The case when input line is invalid (E.g. not ended with a '\n') and
    // the EOF is found at the end of it.
    else if (read_line_state == INPUT_INVALID_AND_EOF) {
      printError();
      break;
    } else if (read_line_state == INPUT_OK) {
      int idx_in_buffer = 0;
      while (inRange('A', 'Z', input_buffer[idx_in_buffer]) ||
             inRange('a', 'z', input_buffer[idx_in_buffer])) {
        ++idx_in_buffer;
      }

      if (input_buffer[idx_in_buffer++] != ' ') {
        // ERROR: Wrong input format; no space after a command.
        printError();
        continue;
      }

      // Arguments for currently called command. None command takes > 2.
      int args[2];

      // Now we call a command based on what stands at the beginning of
      // the input buffer.
      if (prefixMatch(input_buffer, "addUser ")) {
        if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
          printError();
        else
          addUser(tree, args[0], args[1]);
      } else if (prefixMatch(input_buffer, "delUser ")) {
        if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 1, args))
          printError();
        else
          delUser(tree, args[0]);
      } else if (prefixMatch(input_buffer, "addMovie ")) {
        if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
          printError();
        else
          addMovie(tree, args[0], args[1]);
      } else if (prefixMatch(input_buffer, "delMovie ")) {
        if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
          printError();
        else
          delMovie(tree, args[0], args[1]);
      } else if (prefixMatch(input_buffer, "marathon ")) {
        if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
          printError();
        else
          marathon(tree, args[0], args[1]);
      } else {
        // ERROR: Unrecognized opeartion.
        printError();
        continue;
      }
    } else {
      assert(!"Unexpected input state!");
    }
  }

  freeTree(tree);
  return 0;
}
