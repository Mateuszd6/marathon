#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

// Unix line ending character.
#define COMMENT_LINE '#'

// Max size of a VALID input line.
#define MAX_INPUT_LINE_LENGTH 32

// Return values of readInputLine function:
#define INPUT_EOF (-1)
#define INPUT_IGNORED_LINE (0)
#define INPUT_INVALID (-2)
#define INPUT_OK (1)

enum operation { ADD_USER, DEL_USER, ADD_MOVIE, DEL_MOVIE, MARATHON };
typedef enum operation operation_t;

void printError()
{
    fprintf(stderr, "ERROR\n");
}

// Convert number stored as array of chars into int32.
int stringToInt32(const char *str, int len)
{
    if (len > 10 || len <= 0)
        return -1;

    int res = 0;
    for (int i = 0; i < len; ++i)
    {
        assert('0' <= str[i] && str[i] <= '9');

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
    while ('0' <= buffer[(* idx_in_buffer) + letters_read]
        && buffer[(* idx_in_buffer) + letters_read] <= '9')
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
    if (userId) userId++;
    if (parentUserId) parentUserId++;
    assert(!"Requested feature is not implemented!");
}

// Użytkownik o identyfikatorze [userId] wypisuje się. Dodane przez niego
// preferencje filmowe są zapominane. Użytkownicy uprzednio dodani przez
// użytkownika [userId] stają się potomkami rodzica użytkownika
// [userId]. Usunięcie użytkownika ma się wykonywać w czasie stałym. Zapominanie
// preferencji filmowych ma się wykonywać w czasie liniowym względem liczby
// preferencji usuwanego użytkownika.
void delUser (int userId)
{
    if (userId) userId++;
    assert(!"Requested feature is not implemented!");
}

// Użytkownik o identyfikatorze [userId] dodaje film o identyfikatorze
// [movieRating] do swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// dodaje film.
void addMovie (int userId, int movieRating)
{
    if (userId) userId++;
    if (movieRating) movieRating++;
    assert(!"Requested feature is not implemented!");
}

// Użytkownik o identyfikatorze [userId] usuwa film o identyfikatorze
// [movieRating] ze swoich preferencji filmowych. Operacja ma się wykonywać w
// czasie co najwyżej liniowym względem liczby preferencji użytkownika, który
// usuwa film.
void delMovie (int userId, int movieRating)
{
    if (userId) userId++;
    if (movieRating) movieRating++;
    assert(!"Requested feature is not implemented!");
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
    if (userId) userId++;
    if (k) k++;
    assert(!"Requested feature is not implemented!");
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

int main()
{
    int read_line_state = 0;
    while ((read_line_state = readInputLine()) != INPUT_EOF)
    {
        if (read_line_state == INPUT_INVALID)
        {
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
            while (('A' <= input_buffer[idx_in_buffer] && input_buffer[idx_in_buffer] <= 'Z')
                || ('a' <= input_buffer[idx_in_buffer] && input_buffer[idx_in_buffer] <= 'z'))
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
        }
        else
        {
            assert(!"Unexpected input state!");
        }
    }

    return 0;
}
