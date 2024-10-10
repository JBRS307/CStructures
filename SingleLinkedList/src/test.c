#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "single_list.h"

#define SEED 42

void print_int_list(const SingleLinkedList* list) {
    Node* curr = list->head;
    while (curr) {
        int n = *(int*)curr->data;
        printf("%d ", n);
        curr = curr->next;
    }
    putchar('\n');
}

void test_dynamic_allocation_and_free() {

    SingleLinkedList* list = single_list_init(1);
    assert(list != NULL && "test_dynamic_allocation_and_free failed: list is NULL");

    single_list_free(list);
    printf("test_dynamic_allocation_and_free successful!\n");
}

void test_push_and_at() {
    SingleLinkedListStatus status;
    SingleLinkedList list = {0}; // Allocating statically to make things easier
    list.data_size = sizeof(int);

    for (int i = 0; i < 20; i++) {
        status = single_list_push(&list, (void*)&i);
        assert(status == SUCCESS && "test_push_and_at failed: error pushing element");
    }

    int num;
    void* data;
    status = single_list_at(&list, 5, &data);
    num = *(int*)data;
    assert(status == SUCCESS && "test_push_at failed: error getting element!");
    assert(num == 5 && "test_push_at failed: wrong element!");

    status = single_list_at(&list, 100, &data);
    assert(status == LIST_INDEX_OUT_OF_BOUNDS && "test_push_and_at failed: wrong error code returned!");

    printf("test_push_and_at successful!\n");
}


int main() {
    
    
    return EXIT_SUCCESS;
}