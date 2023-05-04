#include "libmx.h"

int tmx_list_size(t_list *list) {
    t_list *temp = list;
    int size = 0;
    if (list == NULL)
        return 0;
    while (temp) {
        temp = temp->next;
        size++;
    }
    return size;
}
