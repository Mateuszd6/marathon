// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef DEBUG
#define NDEBUG
#endif

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "linked_list.h"
#include "tree.h"
#include "utils.h"

// Max size of a VALID input line.
#define MAX_INPUT_LINE_LENGTH (32)

const int MAX_MOVIE_RATING = 2147483647;
const int MAX_K = 2147483647;

// Return values of readInputLine function:
enum input_feedback { INPUT_EOF, INPUT_IGNORED_LINE, INPUT_INVALID, INPUT_INVALID_AND_EOF, INPUT_OK };

// Commands that program supports.
enum program_command { ADD_USER, DEL_USER, ADD_MOVIE, DEL_MOVIE, MARATHON };

static void
printError()
{
    fprintf(stderr, "ERROR\n");
}

static int
readInt32FromBuffer(const char *buffer, int *idx_in_buffer, int *result)
{
    int letters_read = 0;
    while (inRange('0', '9', buffer[(* idx_in_buffer) + letters_read]))
        ++letters_read;

    int res_converted = stringToInt32(buffer + (* idx_in_buffer), letters_read);
    (* idx_in_buffer) += letters_read;
    if (res_converted != -1)
    {
        (* result) = res_converted;
        return 1;
    }
    else
        return 0;
}

static int
readNumbersFromBuffer(const char *buffer, int amount, int *res)
{
    int idx_in_buffer = 0;
    for (int i = 0; i < amount; ++i)
    {
        if (!(readInt32FromBuffer(buffer, &idx_in_buffer, res + i)))
            return 0;
        if (i < amount-1 && buffer[idx_in_buffer++] != ' ')
            return 0;
    }

    return 1;
}

static void
addUser (int parentUserId, int userId)
{
    if (!inRange(0, MAX_USERS, parentUserId) ||
        !inRange(0, MAX_USERS, userId) ||
        !treeAddNode(userId, parentUserId))
        printError();
    else
        printf("OK\n");
}

static void
delUser (int userId)
{
    if (!inRange(0, MAX_USERS, userId) || !treeDelNode(userId))
        printError();
    else
        printf("OK\n");
}

static void
addMovie (int userId, int movieRating)
{

    if (!inRange(0, MAX_USERS, userId) ||
        !inRange(0, MAX_MOVIE_RATING, movieRating) ||
        !treeAddPreference(userId, movieRating))
        printError();
    else
        printf("OK\n");
}

static void
delMovie (int userId, int movieRating)
{
    if (!inRange(0, MAX_USERS, userId) ||
        !inRange(0, MAX_MOVIE_RATING, movieRating) ||
        !treeRemovePreference(userId, movieRating))
        printError();
    else
        printf("OK\n");
}

static void
marathon (int userId, int k)
{
    if (!inRange(0, MAX_USERS, userId) || !inRange(0, MAX_K, k))
    {
        printError();
        return;
    }

#ifdef DEBUG
    printf("Marathon on tree:\n");
    printTree();
#endif

    struct List *res = runMarathon(userId, k);
    if (!res)
    {
        printError();
    }
    else
    {
        if (listEmpty(res))
            printf("NONE\n");
        else
        {
            listPrintContent(res);
            printf("\n");
        }

        listFree(res);
    }
}

// [MAX_INPUT_LINE_LENGTH] characters is more than enought for valid,
// non-comment input lines. Comment lines are ignored, never stored in buffer.
static char input_buffer [MAX_INPUT_LINE_LENGTH];

static enum input_feedback
readInputLine()
{
    char c;
    int index_in_buffer = 0;

    switch(c = getchar())
    {
    case EOF:
        return INPUT_EOF;

    case '\n':
        return INPUT_IGNORED_LINE;

    // If given line is a comment, just skip to the EOL.
    case '#':
    {
        while ((c = getchar()) != '\n')
        {
            if (c == EOF)
                // TODO: SHOULD THIS RETURN INPUT_INVALID_AND_EOF,
                // or do we accept it (return INPUT_EOF)?
                return INPUT_INVALID_AND_EOF;
        }

        return INPUT_IGNORED_LINE;
    }

    default:
    {
        int buffer_size = 0;
        // Don't forget about the first character (getchar in switch statement).
        do
        {
            // Lines must end with '\n' so the input is invalid!
            if (c == EOF)
                return INPUT_INVALID_AND_EOF;

            if (buffer_size >= MAX_INPUT_LINE_LENGTH)
                break;

            input_buffer[index_in_buffer++] = c;
            buffer_size++;
        }
        while ((c = getchar()) != '\n');

        // [MAX_INPUT_LINE_LENGTH] is large enough to store any vaild input so
        // the inserted line IS NOT valid. NOTE: Trailing zeors are not supported!
        if (c != '\n')
        {
            // Skip to the end of the line and return an input error.
            while ((c = getchar()) != '\n')
                if (c == EOF)
                    return INPUT_INVALID_AND_EOF;

            return INPUT_INVALID;
        }

        input_buffer[index_in_buffer] = '\0';
        return INPUT_OK;
    }
    }
}

int
main(void)
{
    initTree();
    int read_line_state = 0;
    while ((read_line_state = readInputLine()) != INPUT_EOF)
    {
        if (read_line_state == INPUT_INVALID)
        {
            // ERROR: Invalid input.
            printError();
            continue;
        }
        else if (read_line_state == INPUT_IGNORED_LINE)
        {
            continue;
        }
        // The case when input line is invalid (E.g. not ended with a '\n') and
        // the EOF is found at the end of it.
        else if (read_line_state == INPUT_INVALID_AND_EOF)
        {
            printError();
            break;
        }
        else if (read_line_state == INPUT_OK)
        {
            int idx_in_buffer = 0;
            while (inRange('A', 'Z', input_buffer[idx_in_buffer])
                || inRange('a', 'z', input_buffer[idx_in_buffer]))
            {
                ++idx_in_buffer;
            }

            if (input_buffer[idx_in_buffer++] != ' ')
            {
                // ERROR: Wrong input format; no space after a command.
                printError();
                continue;
            }

            // Currently called program command and its arguments.
            enum program_command op;
            int args[2];

            if (prefixMatch(input_buffer, "addUser"))
                op = ADD_USER;
            else if (prefixMatch(input_buffer, "delUser"))
                op = DEL_USER;
            else if (prefixMatch(input_buffer, "addMovie"))
                op = ADD_MOVIE;
            else if (prefixMatch(input_buffer, "delMovie"))
                op = DEL_MOVIE;
            else if (prefixMatch(input_buffer, "marathon"))
                op = MARATHON;
            else
            {
                // ERROR: Unrecognized opeartion.
                printError();
                continue;
            }

            switch(op)
            {
            case ADD_USER:
            {
                if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
                    printError();
                else
                    addUser(args[0], args[1]);
            } break;

            case DEL_USER:
            {
                if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 1, args))
                    printError();
                else
                    delUser(args[0]);
            } break;


            case ADD_MOVIE:
            {
                if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
                    printError();
                else
                    addMovie(args[0], args[1]);
            } break;

            case DEL_MOVIE:
            {
                if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
                    printError();
                else
                    delMovie(args[0], args[1]);
            } break;

            case MARATHON:
            {
                if (!readNumbersFromBuffer(input_buffer + idx_in_buffer, 2, args))
                    printError();
                else
                    marathon(args[0], args[1]);
            } break;

            }
        }
        else
        {
            assert(!"Unexpected input state!");
        }
    }

    freeTree();
    return 0;
}
