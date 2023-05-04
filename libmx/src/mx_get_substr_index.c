#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (!str || !sub) {
        return -2;
    }
    for (int i = 0; i < mx_strlen(str); i++) {
        if (mx_strncmp(&str[i], sub, mx_strlen(sub))) {
            return i;
        }
    }
    return -1;
}
