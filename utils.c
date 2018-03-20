#include <limits.h>
// Dont include asserts in the release build.
#if !defined DEBUG
#define NDEBUG
#endif
#include <assert.h>

int inRange(const int min, const int max, const int value)
{
    assert(min <= max);
    return (min <= value && value <= max);
}

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

int prefixMatch(const char *text, const char *pattern)
{
    for (int i = 0; pattern[i] != '\0'; ++i)
        if (text[i] != pattern[i])
            return 0;

    return 1;
}
