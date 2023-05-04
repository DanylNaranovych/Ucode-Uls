#include <stdio.h>

char *mx_strchr(const char *s, int c) {
  char ch;
  ch = c;
  for (;; ++s) {
    if (*s == ch)
      return ((char *)s);
    if (*s == '\0')
      return (NULL);
  }
}
