// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#include "linked_list.h"
#include "tree.h"

// Unix line ending character.
#define COMMENT_LINE '#'

// Max size of a VALID input line.
#define MAX_INPUT_LINE_LENGTH (32)

const int MAX_MOVIE_RATING = 2147483647;
const int MAX_K = 2147483647;

// Return values of readInputLine function:
enum input_feedback { INPUT_EOF, INPUT_IGNORED_LINE, INPUT_INVALID, INPUT_OK };
typedef enum input_feedback input_feedback_t;

enum operation { ADD_USER, DEL_USER, ADD_MOVIE, DEL_MOVIE, MARATHON };
typedef enum operation operation_t;

void printError()
{
    fprintf(stderr, "ERROR\n");
}

// 1 if 'min <= value <= max', 0 in other case.
int inRange(const int min, const int max, const int value)
{
    assert(min <= max);
    return (min <= value && value <= max);
}

// Convert number stored as array of chars into int32.
int stringToInt32(const char *str, int len)
{
    if (!inRange(1, 10, len))
        return -1;

    int res = 0;
    for (int i = 0; i < len; ++i)
    {
        assert(inRange('0', '9', str[i]));

        // Check borders when we are close to INT_MAX.
        if (i == 9 && (res > INT_MAX / 10
                || (res == INT_MAX / 10 && str[i] - '0' > INT_MAX % 10)))
        {
            return -1;
        }

        res *= 10;
        res += str[i] - '0';
    }

    return res;
}

// Check if [pattern] is a prefix of [text].
// NOTE: pattern must be a string literal.
int prefixMatch(const char *text, const char *pattern)
{
    for (int i = 0; pattern[i] != '\0'; ++i)
        if (text[i] != pattern[i])
            return 0;

    return 1;
}

int readInt32FromBuffer(const char *buffer, int *idx_in_buffer, int *result)
{
    int letters_read = 0;
    while (inRange('0', '9', buffer[(* idx_in_buffer) + letters_read]))
    {
        ++letters_read;
    }

    int res_converted = stringToInt32(buffer + (* idx_in_buffer), letters_read);
    (* idx_in_buffer) += letters_read;
    if (res_converted != -1)
    {
        (* result) = res_converted;
        return 1;
    }

    return 0;
}

// Użytkownik o identyfikatorze [parentUserId] dodaje użytkownika o
// identyfikatorze [userId]. Operacja ma się wykonywać w czasie stałym.
void addUser (int parentUserId, int userId)
{
    if (!treeAddNode(userId, parentUserId))
        printError();
}

// Użytkownik o identyfikatorze [userId] wypisuje się. Dodane przez niego
// preferencje filmowe są zapominane. Użytkownicy uprzednio dodani przez
// użytkownika [userId] stają się potomkami rodzica użytkownika
// [userId]. Usunięcie użytkownika ma się wykonywać w czasie stałym. Zapominanie
// preferencji filmowych ma się wykonywać w czasie liniowym względem liczby
// preferencji usuwanego użytkownika.
void delUser (int userId)
{
    if (!treeDelNode(userId))
        printError();
}

// Użytkownik o identyfikatorze [userId] dodaje film o identyfikatorze
// [movieRating] do swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// dodaje film.
void addMovie (int userId, int movieRating)
{
    if (!treeAddPreference(userId, movieRating))
        printError();
}

// Użytkownik o identyfikatorze [userId] usuwa film o identyfikatorze
// [movieRating] ze swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// usuwa film.
void delMovie (int userId, int movieRating)
{
    if (!treeRemovePreference(userId, movieRating))
        printError();
}

// Wyznacza co najwyżej [k] identyfikatorów filmów o najwyższych ocenach
// spośród:
// 1) własnych preferencji filmowych użytkownika o identyfikatorze [userId];
// 2) preferencji filmowych wyodrębnionych w wyniku przeprowadzenia operacji
// [marathon] dla każdego z potomków użytkownika userId, przy czym w wynikowej
// grupie [k] filmów znajdą się tylko takie, które mają ocenę większą od
// maksymalnej oceny filmu spośród preferencji użytkownika userId.
void marathon (int userId, int k)
{
    printf("Marathon on tree:\n");
    printTree();

    
}

// [MAX_INPUT_LINE_LENGTH] characters is more than enought for valid,
// non-comment input lines. Comment lines are ignored and never stored in
// buffer.
char input_buffer[MAX_INPUT_LINE_LENGTH];

int readInputLine()
{
    char c;
    int index_in_buffer = 0;

    // TODO: Can "empty lines" have whitespaces?

    switch(c = getchar())
    {
    case EOF:
        return INPUT_EOF;

    case '\n':
        return INPUT_IGNORED_LINE;

    // If given line is a comment, just skip to the EOL.
    case COMMENT_LINE:
    {
        while ((c = getchar()) != '\n')
        {
            // TODO: If c == EOF line is invalid!
            assert(c != EOF);
        }
        return INPUT_IGNORED_LINE;
    }

    default:
    {
        int buffer_size = 0;
        do
        {
            // TODO: If c == EOF line is invalid!
            assert(c != EOF);

            input_buffer[index_in_buffer++] = c;
            buffer_size++;
        }
        while ((c = getchar()) != '\n'
            && buffer_size < MAX_INPUT_LINE_LENGTH);

        if (c != '\n')
        {
            // MAX_INPUT_LINE_LENGTH is large enough to store any vaild input
            // so the inserted line is not valid.

            // Skip to the end of the line and return an input error.
            while ((c = getchar()) != '\n')
            {
                // TODO: If c == EOF line is invalid! (It is already!!!!)
                assert(c != EOF);
            }
            return INPUT_INVALID;
        }

        input_buffer[index_in_buffer] = '\0';
        return INPUT_OK;
    }
    }
}

void executeComamnd(operation_t call, int* args)
{
    switch(call)
    {
    case ADD_USER:
    {
        if (!inRange(0, MAX_USERS, args[0]) || !inRange(0, MAX_USERS, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        addUser(args[0], args[1]);
    } break;

    case DEL_USER:
    {
        if (!inRange(0, MAX_USERS, args[0]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        delUser(args[0]);
    } break;

    case ADD_MOVIE:
    {
        if (!inRange(0, MAX_USERS, args[0]) || !inRange(0, MAX_MOVIE_RATING, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        addMovie(args[0], args[1]);
    } break;

    case DEL_MOVIE:
    {
        if (!inRange(0, MAX_USERS, args[0]) || !inRange(0, MAX_MOVIE_RATING, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        delMovie(args[0], args[1]);
    } break;

    case MARATHON:
    {
        if (!inRange(0, MAX_USERS, args[0]) || !inRange(0, MAX_K, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        // TODO: Check if:
        // *) args[0] exist in the tree.
        marathon(args[0], args[1]);
    } break;
    }
}

int main (void)
{
// TODO: Remove or move to the test.
#ifdef DEBUG
    {
        // Some tests for the lists.
        struct List *my_list = malloc(sizeof(struct List)),
            *my_other_list = malloc(sizeof(struct List));

        if (!my_list || !my_other_list)
            exit(1);

        listPushBack(my_list, 12);
        listPushBack(my_list, 8);
        listPushBack(my_list, 5);
        /* listPushBack(my_list, 2); */
        /* listPushBack(my_list, 1); */
        listInsertMaintainSortOrder(my_list, 3);
        listInsertMaintainSortOrder(my_list, 3);
        listInsertMaintainSortOrder(my_list, 18);
        assert(listRemoveElement(my_list, 5));
        assert(listRemoveElement(my_list, 18));
        assert(listRemoveElement(my_list, 12));
        assert(listRemoveElement(my_list, 8));
        assert(listRemoveElement(my_list, 3));
        assert(listEmpty(my_list));

        listPushBack(my_other_list, 0);
        listPushBack(my_other_list, -1);
        listPushBack(my_other_list, -5);
        listPushBack(my_other_list, -20);
        listPushBack(my_other_list, -1236);
        assert(!listRemoveElement(my_other_list, 1));

        printf("My list: ");
        listPrintContent(my_list);
        printf("\n");
        assert(listIsSorted(my_list));

        printf("Other list: ");
        listPrintContent(my_other_list);
        printf("\n");
        assert(listIsSorted(my_list));

        listConcat(my_list, my_other_list);

        printf("\nAfter concat:\n");
        printf("My list: ");
        listPrintContent(my_list);
        printf("\n");
        assert(listIsSorted(my_list));

        printf("Other list: ");
        listPrintContent(my_other_list);
        printf("\n");
        assert(listIsSorted(my_list));

        listFree(my_list);
        free(my_other_list);

        // Tree tests:
        initTree();

        treeAddNode(1, 0);
        treeAddNode(2, 1);
        treeAddNode(4, 1);
        treeAddNode(7, 1);
        treeAddNode(10, 1);
        treeAddNode(3, 2);
        treeAddNode(5, 2);
        treeAddNode(6, 0);
        treeAddNode(8, 0);

        printTree();

        treeDelNode(1);
        printTree();

        freeTree();
    }
#endif

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
        else if (read_line_state == INPUT_OK)
        {
            int idx_in_buffer = 0;
            while (inRange('A', 'Z', input_buffer[idx_in_buffer])
                || inRange('a', 'z', input_buffer[idx_in_buffer]))
            {
                ++idx_in_buffer;
            }

            // TODO: Handle invalid input.
            if (input_buffer[idx_in_buffer++] != ' ')
            {
                // ERROR: Wrong input format; no space after a command.
                printError();
                continue;
            }

            // Currently called operation and its arguments.
            operation_t op;
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

            switch (op)
            {
            case ADD_USER:
            case ADD_MOVIE:
            case DEL_MOVIE:
            case MARATHON:
            {
                // If ANY of the following function fails input is invalid.
                if (!(readInt32FromBuffer(input_buffer, &idx_in_buffer, args)
                        && input_buffer[idx_in_buffer++] == ' '
                        && readInt32FromBuffer(input_buffer, &idx_in_buffer, args+1)
                        && input_buffer[idx_in_buffer++] == '\0'))
                {
                    // ERROR: Invalid input.
                    printError();
                    continue;
                }
            } break;

            case DEL_USER:
            {
                // If ANY of the following function fails input is invalid.
                if (!(readInt32FromBuffer(input_buffer, &idx_in_buffer, args)
                        && input_buffer[idx_in_buffer++] == '\0'))
                {
                    // ERROR: Invalid input.
                    printError();
                    continue;
                }
            } break;
            }

            executeComamnd(op, args);
        }
        else
        {
            assert(!"Unexpected input state!");
        }
    }

    freeTree();
    return 0;
}
