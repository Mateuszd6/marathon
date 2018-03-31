// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef DEBUG
#define NDEBUG
#endif

#include <assert.h>
#include <limits.h>

#include "utils.h"

// Convert number stored as array of chars into int32. Return -1 when a numer is
// wrong format, or is greater then INT_MAX. Trailing zeros are not supported!
static int stringToInt32(const char *str, int len) {
  // For sure in in range of int32. Dont allow for trailing zeros.
  if (!inRange(1, 10, len) || (str[0] == '0' && len > 1))
    return -1;

  int res = 0;
  for (int i = 0; i < len; ++i) {
    assert(inRange('0', '9', str[i]));

    // Check borders when we are close to INT_MAX.
    if (i == 9 && (res > INT_MAX / 10 ||
                   (res == INT_MAX / 10 && str[i] - '0' > INT_MAX % 10))) {
      return -1;
    }

    res *= 10;
    res += str[i] - '0';
  }

  return res;
}

// Read int from a [buffer], staring from [idx_in_buffer]. Store value in
// [result]. If failed, returns 0, else 1.
static int readInt32FromBuffer(const char *buffer, int *idx_in_buffer,
                               int *result) {
  int letters_read = 0;
  while (inRange('0', '9', buffer[(*idx_in_buffer) + letters_read]))
    ++letters_read;

  int res_converted = stringToInt32(buffer + (*idx_in_buffer), letters_read);
  (*idx_in_buffer) += letters_read;
  if (res_converted != -1) {
    (*result) = res_converted;
    return 1;
  } else
    return 0;
}

int readNumbersFromBuffer(const char *buffer, int amount, int *res) {
  int idx_in_buffer = 0;
  for (int i = 0; i < amount; ++i) {
    if (!(readInt32FromBuffer(buffer, &idx_in_buffer, res + i)))
      return 0;
    if (i < amount - 1 && buffer[idx_in_buffer++] != ' ')
      return 0;
  }

  return 1;
}

int inRange(const int min, const int max, const int value) {
  assert(min <= max);
  return (min <= value && value <= max);
}

int prefixMatch(const char *text, const char *pattern) {
  for (int i = 0; pattern[i] != '\0'; ++i)
    if (text[i] != pattern[i])
      return 0;

  return 1;
}
