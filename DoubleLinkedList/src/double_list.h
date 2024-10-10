#ifndef __DOUBLE_LIST
#define __DOUBLE_LIST

#include <sys/types.h>

typedef enum {
    SUCCESS,
    MEMORY_ALLOCATION_ERROR,
    LIST_INDEX_OUT_OF_BOUNDS,
    NULL_DATA_POINTER,
} DoubleLinkedListStatus;

typedef struct Node Node;
struct Node {
    void* data;
    Node* next;
    Node* prev;
};

typedef int (*Comparator)(const void*, const void*);

typedef struct {
    Node* head;
    Node* tail;
    Comparator compar;
    size_t size;
    size_t data_size;
} DoubleLinkedList;

// Dynamically allocates list with given data size
DoubleLinkedList* double_list_init(size_t data_size);

// Frees list allocated with double_list_init
void double_list_free(DoubleLinkedList* list);

#endif