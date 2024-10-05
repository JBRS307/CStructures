#include <stdio.h>
#include <stdlib.h>

#include "single_list.h"

int main() {
    SingleLinkedList* list = single_list_init(sizeof(int));

    for (int i = 1; i <= 15; i++) {
        if (single_list_push(list, (void*)&i) != SUCCESS) {
            exit(EXIT_FAILURE);
        }
    }

    void* data;
    if (single_list_at(list, 22, &data) != SUCCESS) {
        fprintf(stderr, "Error in list!\n");
    }

    single_list_at(list, 3, &data);

    int d = *(int*)data;

    printf("%d\n", d);

    return EXIT_SUCCESS;
}