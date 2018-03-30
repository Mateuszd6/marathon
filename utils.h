// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef UTILS_H
#define UTILS_H

#define SWAP(first, second)                     \
    ({ __typeof(first) __swap_v = first;        \
        first = second;                         \
        second = __swap_v;                      \
    })

#define MAX(first, second)                      \
    ({ __typeof((first)) _a = (first);          \
        __typeof((second)) _b = (second);       \
        _a >= _b ? _a : _b;                     \
    })

// 1 if 'min <= value <= max', 0 in other case. Assumes min <= max.
int inRange(const int min, const int max, const int value);

// Check if [pattern] is a prefix of [text].
// [pattern] must terminate with '\0'.
int prefixMatch(const char *text, const char *pattern);

// Read [amount] of numbers from a [buffer], and store them in [res]. Assumes
// that there is enough space allocated at [res] to store all numbres. values in
// the buffer must be separated with a single space. Returns 1on sucess, else 0.
int readNumbersFromBuffer(const char *buffer, int amount, int *res);

#endif
