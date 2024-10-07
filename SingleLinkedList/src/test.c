#include <stdio.h>
#include <stdlib.h>

#include "single_list.h"

void print_int_list(const SingleLinkedList* list) {
    Node* curr = list->head;
    while (curr) {
        int n = *(int*)curr->data;
        printf("%d ", n);
        curr = curr->next;
    }
    putchar('\n');
}

int main() {
    SingleLinkedList* list = single_list_init(sizeof(int));

    for (int i = 1; i <= 15; i++) {
        if (single_list_push(list, (void*)&i) != SUCCESS) {
            exit(EXIT_FAILURE);
        }
    }

    print_int_list(list);
    // single_list_reverse(list);
    // print_int_list(list);

    single_list_free(list);

    return EXIT_SUCCESS;
}