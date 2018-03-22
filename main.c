// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
// Dont include asserts in the release build.
#if !defined DEBUG
#define NDEBUG
#endif
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
typedef enum input_feedback input_feedback_t;

enum operation { ADD_USER, DEL_USER, ADD_MOVIE, DEL_MOVIE, MARATHON };
typedef enum operation operation_t;

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

    return 0;
}

// Użytkownik o identyfikatorze [parentUserId] dodaje użytkownika o
// identyfikatorze [userId]. Operacja ma się wykonywać w czasie stałym.
static void
addUser (int parentUserId, int userId)
{
    if (!treeAddNode(userId, parentUserId))
        printError();
    else
        printf("OK\n");
}

// Użytkownik o identyfikatorze [userId] wypisuje się. Dodane przez niego
// preferencje filmowe są zapominane. Użytkownicy uprzednio dodani przez
// użytkownika [userId] stają się potomkami rodzica użytkownika
// [userId]. Usunięcie użytkownika ma się wykonywać w czasie stałym. Zapominanie
// preferencji filmowych ma się wykonywać w czasie liniowym względem liczby
// preferencji usuwanego użytkownika.
static void
delUser (int userId)
{
    if (!treeDelNode(userId))
        printError();
    else

        printf("OK\n");}

// Użytkownik o identyfikatorze [userId] dodaje film o identyfikatorze
// [movieRating] do swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// dodaje film.
static void
addMovie (int userId, int movieRating)
{
    if (!treeAddPreference(userId, movieRating))
        printError();
    else
        printf("OK\n");
}

// Użytkownik o identyfikatorze [userId] usuwa film o identyfikatorze
// [movieRating] ze swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// usuwa film.
static void
delMovie (int userId, int movieRating)
{
    if (!treeRemovePreference(userId, movieRating))
        printError();
    else
        printf("OK\n");
}

// Wyznacza co najwyżej [k] identyfikatorów filmów o najwyższych ocenach
// spośród:
// 1) własnych preferencji filmowych użytkownika o identyfikatorze [userId];
// 2) preferencji filmowych wyodrębnionych w wyniku przeprowadzenia operacji
// [marathon] dla każdego z potomków użytkownika userId, przy czym w wynikowej
// grupie [k] filmów znajdą się tylko takie, które mają ocenę większą od
// maksymalnej oceny filmu spośród preferencji użytkownika userId.
static void
marathon (int userId, int k)
{
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

// `MAX_INPUT_LINE_LENGTH` characters is more than enought for valid,
// non-comment input lines. Comment lines are ignored, never stored in buffer.
char input_buffer[MAX_INPUT_LINE_LENGTH];

static int
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
        // Don't forget about the first character (getchar in swtich statement).
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

        // `MAX_INPUT_LINE_LENGTH` is large enough to store any vaild input so
        // the inserted line IS NOT valid.
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

static void
executeComamnd(operation_t call, int* args)
{
    switch(call)
    {
    case ADD_USER:
    {
        if (!inRange(0, MAX_USERS, args[0])
            || !inRange(0, MAX_USERS, args[1]))
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
        if (!inRange(0, MAX_USERS, args[0])
            || !inRange(0, MAX_MOVIE_RATING, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        addMovie(args[0], args[1]);
    } break;

    case DEL_MOVIE:
    {
        if (!inRange(0, MAX_USERS, args[0])
            || !inRange(0, MAX_MOVIE_RATING, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        delMovie(args[0], args[1]);
    } break;

    case MARATHON:
    {
        if (!inRange(0, MAX_USERS, args[0])
            || !inRange(0, MAX_K, args[1]))
        {
            // ERROR: argument(s) out of range.
            printError();
        }

        marathon(args[0], args[1]);
    } break;
    }
}

int
main(void)
{
#ifdef DEBUG
    {
        // Some tests for the lists.
        struct List *my_list = malloc(sizeof(struct List)),
            *my_other_list = malloc(sizeof(struct List));

        if (!my_list || !my_other_list)
            exit(1);

        assert(listInsertMaintainSortOrder(my_list, 12));
        listPushBack(my_list, 8);
        listPushBack(my_list, 5);
        /* listPushBack(my_list, 2); */
        listPushBack(my_list, 1);
        assert(listInsertMaintainSortOrder(my_list, 3));
        assert(!listInsertMaintainSortOrder(my_list, 3));
        listInsertMaintainSortOrder(my_list, 18);

        printf("Lets start: ");
        listPrintContent(my_list);
        printf("\n");
        assert(listRemoveElement(my_list, 1));
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

        // Some tests for the lists.
        my_list = malloc(sizeof(struct List));
        my_other_list = malloc(sizeof(struct List));

        if (!my_list || !my_other_list)
            exit(1);

        (* my_list) = (struct List){ NULL, NULL };
        (* my_other_list) = (struct List){ NULL, NULL };

        listPushBack(my_list, 12);
        listPushBack(my_list, 8);
        listPushBack(my_list, 5);
        listInsertMaintainSortOrder(my_list, 3);
        listInsertMaintainSortOrder(my_list, 18);
        assert(listRemoveElement(my_list, 5));

        listPushBack(my_other_list, 11);
        listPushBack(my_other_list, 9);
        listPushBack(my_other_list, 6);
        listPushBack(my_other_list, 0);
        listPushBack(my_other_list, -1);
        listPushBack(my_other_list, -5);
        listPushBack(my_other_list, -20);
        listPushBack(my_other_list, -1236);
        assert(!listRemoveElement(my_other_list, 1));

        printf("\nOnce again:\n");
        listPrintContent(my_list);
        printf("\n");
        listPrintContent(my_other_list);
        printf("\n\n");

        struct List *merged = listMergeSortedLists(my_list, my_other_list, 2, 3);

        printf("Merged: with 2, 3: ");
        listPrintContent(merged);
        printf("\n\n");

        listFree(merged);

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

    // TODO: JUST REMOVE IT LATER.
    {
        int *trap = malloc(sizeof(int) * 40);
        trap[9] = rand() % 2;
        if (trap[9] == 3)
            exit(0);

        printf("Trap!\n");
    }

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
