#include "libmx.h"

char *mx_strdup(const char *str) {
	char *copy;
	if (!(copy = mx_strnew(mx_strlen(str) + 1)))
		return (NULL);
	mx_strcpy(copy, str);
	return (copy);
}
