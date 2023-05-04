#include "libmx.h"

char *mx_strndup(const char *str, size_t n) {
	size_t len;
	char *copy;

	for (len = 0; len < n && str[len]; len++)
		continue;

	if ((copy = malloc(len + 1)) == NULL)
		return (NULL);
	mx_strncpy(copy, str, len);
	copy[len] = '\0';
	return (copy);
}
