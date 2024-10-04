#include <stdlib.h>
#include <string.h>

#include "single_list.h"

SingleLinkedList* single_list_init(size_t data_size) {
    if (data_size == 0) {
        return NULL;
    }

    SingleLinkedList* new = (SingleLinkedList*)malloc(sizeof(SingleLinkedList));
    if (!new) {
        return NULL;
    }

    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    new->data_size = data_size;
    return new;
}

SingleLinkedListStatus single_list_push(SingleLinkedList* list, const void* data) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (!new) {
        return MEMORY_ALLOCATION_ERROR;
    }

    new->data = malloc(list->data_size);
    if (!new->data) {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }
    new->next = NULL;
    
    memcpy(new->data, data, list->data_size);

    if (!list->tail) {
        list->head = list->tail = new;
    } else {
        list->tail->next = new;
        list->tail = new;
    }
    list->size++;
    return SUCCESS;
}

SingleLinkedListStatus single_list_at(const SingleLinkedList* list, size_t idx, void** data) {
    if (idx >= list->size) {
        return LIST_INDEX_OUT_OF_BOUNDS;
    }
    if (idx == list->size - 1) {
        *data = list->tail->data;
        return SUCCESS;
    }

    Node* curr = list->head;
    size_t i = 0;
    while (i < idx) {
        curr = curr->next;
        i++;
    }

    *data = curr->data;
    return SUCCESS;
}

SingleLinkedListStatus single_list_insert(SingleLinkedList* list, size_t idx, const void* data) {
    if (idx >= list->size || list->size == 0) {
        return single_list_push(list, data);
    }
    
    Node* new = (Node*)malloc(sizeof(Node));
    if (!new) {
        return MEMORY_ALLOCATION_ERROR;
    }

    new->data = malloc(list->data_size);
    if (!new->data) {
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new->data, data, list->data_size);

    if (idx == 0) {
        new->next = list->head;
        list->head = new;
    } else {
        Node* insert_after = list->head;
        size_t i = 1;
        while (i < idx) {
            i++;
            insert_after = insert_after->next;
        }
        new->next = insert_after->next;
        insert_after->next = new;
    }
    list->size++;
    return SUCCESS;
}