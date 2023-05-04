#include "libmx.h"

int mx_lower_strcmp(const char *s1, const char *s2) {

  char *str1 = mx_strdup(s1);
  char *str2 = mx_strdup(s2);

  if (mx_isupper(*str1))
    *str1 = mx_tolower(*str1);

  if (mx_isupper(*str2))
    *str2 = mx_tolower(*str2);

  while (*str1 == *str2) {
    if (*str1 == '\0' && *str2 == '\0') {
      return 0;
    }

    if (mx_isupper(*str1))
      *str1 = mx_tolower(*str1);

    if (mx_isupper(*str2))
      *str2 = mx_tolower(*str2);

    str1++;
    str2++;
  }

  return *str1 - *str2;
}
