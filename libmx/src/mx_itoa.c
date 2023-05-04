#include "libmx.h"

char *mx_itoa(long long number) {
    char *word = NULL;
    int size = 0;
    long long copy;
    if (number == 0) {
        word = mx_strnew(1);
        word[0] = '0';
        return word;
    }
    copy = number;
    if (copy < 0) {
        size++;
        copy = -copy;
    }
    while(copy != 0) {
        copy /= 10;
        size++;
    }
    word = mx_strnew(size);
    if (number < 0) {
        word[0] = '-';
        number = -number;
    }
    word[size--] = '\0';
    while ((number != 0 && size >= 0) && (word[size] != '-')) {
        word[size--] = (number % 10) + '0';
        number /= 10;
    }
    return word;
}
