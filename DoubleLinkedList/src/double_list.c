#include <stdio.h>
#include <stdlib.h>

#include "double_list.h"

//-------------------------STATICS--------------------------------

static void s_free_node(Node* node) {
    free(node->data);
    free(node);
}

static void s_free_all_nodes(DoubleLinkedList* list) {
    Node* curr = list->head;
    while (curr->next) {
        Node* to_free = curr;
        curr = curr->next;
        s_free_node(curr);
    }
    s_free_node(curr);
}

//--------------------------CREATE-DELETE-----------------------------

DoubleLinkedList* double_list_init(size_t data_size) {
    DoubleLinkedList* new = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
    if (!new) {
        return NULL;
    }

    new->head = NULL;
    new->tail = NULL;
    new->compar = NULL;
    new->size = 0;
    new->data_size = data_size;

    return new;
}

void double_list_free(DoubleLinkedList* list) {
    if (!list->head) {
        free(list);
        return;
    }

    s_free_all_nodes(list);
    free(list);
}