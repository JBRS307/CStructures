#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int compare_int(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;

    return a - b;
} 

int main() {
    srand(time(NULL));
    SingleLinkedList* list = single_list_init(sizeof(int));
    list->compar = compare_int;

    for (int i = 1; i <= 130; i++) {
        int elem = rand() % 20 + 1;
        if (single_list_push(list, (void*)&elem) != SUCCESS) {
            exit(EXIT_FAILURE);
        }
    }

    print_int_list(list);
    
    if (single_list_sort_desc(list) != SUCCESS) {
        exit(EXIT_FAILURE);
    }
    print_int_list(list);

    single_list_free(list);

    return EXIT_SUCCESS;
}