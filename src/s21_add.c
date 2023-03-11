#include "s21_sprintf.h"

// char *strtok(char *str, const char *delim)
// char *strerror(int errnum)
// char *strcat(char *dest, const char *src)
// char *strncat(char *dest, const char *src, s21_size_t n)

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t dest_len = s21_strlen(dest);
  s21_size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[dest_len + i] = src[i];
  }
  dest[dest_len + i] = '\0';

  return dest;
}

char *s21_strcat(char *restrict dest, const char *restrict src) {
  return s21_strncat(dest, src, s21_strlen(src));
}

unsigned int is_char_in(const char *str, char c) {
  for (; *str != '\0'; ++str) {
    if (*str == c) {
      return 1;
    }
  }
  return 0;
}

char *s21_strtok(char *restrict str, const char *restrict sep) {
  static char *last_stop = S21_NULL;

  if (str == S21_NULL) {
    str = last_stop;
    if (str == S21_NULL) {
      return S21_NULL;
    }
  }

  str += s21_strspn(str, sep);
  if (*str == '\0') {
    last_stop = str;
    return S21_NULL;
  }

  char *res = str;
  while (1) {
    if (*str == '\0') {
      last_stop = str;
      return res;
    }
    if (is_char_in(sep, *str)) {
      *str = '\0';
      last_stop = str + 1;
      return res;
    }
    str++;
  }
}

char *s21_strerror(int errnum) {
  char *description = S21_NULL;
  static char default_description[100];

  int found = 0;
  for (int i = 0; i < s21_sys_nerr && !found; ++i) {
    if (s21_sys_errlist[i].key == errnum) {
      found = 1;
      description = s21_sys_errlist[i].value;
    }
  }

  if (!found) {
    s21_memset(default_description, '\0', 100);
#if defined(__linux__)
    s21_sprintf(default_description, "Unknown error %d", errnum);
#elif __APPLE__
    s21_sprintf(default_description, "Unknown error: %d", errnum);
#endif
    description = default_description;
  }

  return description;
}
