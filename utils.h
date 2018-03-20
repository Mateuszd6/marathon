#ifndef UTILS_H
#define UTILS_H

#define SWAP(first, second)                                      \
    ({ __typeof(first) __swap_v = first;                         \
        first = second;                                          \
        second = __swap_v;                                       \
    })

#define MAX(first, second)                                       \
    ({ __typeof((first)) _a = (first);                           \
        __typeof((second)) _b = (second);                        \
        _a >= _b ? _a : _b;                                      \
    })

#endif
