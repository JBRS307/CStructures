#ifndef __SINGLE_LIST
#define __SINGLE_LIST

#include <sys/types.h>

typedef enum {
    SUCCESS,
    MEMORY_ALLOCATION_ERROR,
    LIST_INDEX_OUT_OF_BOUNDS,
    LIST_EMPTY,
    NULL_DATA_POINTER,
    COMPARATOR_FUNCTION_MISSING,
} SingleLinkedListStatus;

typedef struct Node Node;
struct Node {
    void* data;
    Node* next;    
};

typedef int (*Comparator)(const void*, const void*);

typedef struct {
    Node* head;
    Node* tail;
    size_t size;
    size_t data_size;
    Comparator compar;
} SingleLinkedList;

// Dynamically initializes empty single linked list
// with given data_size, returns NULL on error
SingleLinkedList* single_list_init(size_t data_size);

// Frees memory occupied by dynamically allocated list and all its nodes
void single_list_free(SingleLinkedList* list);

// Saves address pointed to by node at index idx into data
// Allows modification of data
SingleLinkedListStatus single_list_at(const SingleLinkedList* list, size_t idx, void** data);

// Saves pointer to max element (according to list compar function) to data pointer
SingleLinkedListStatus single_list_max(const SingleLinkedList* list, void** data);

// Saves pointer to min element (according to list compar function) to data pointer
SingleLinkedListStatus single_list_min(const SingleLinkedList* list, void** data);

// Adds data to the end of the list. Copies at most data_size bytes
// from memory pointed by data into the node at the end of the list
SingleLinkedListStatus single_list_push(SingleLinkedList* list, const void* data);

// Inserts element before node at index idx. If idx >= list->size element is pushed at the end
SingleLinkedListStatus single_list_insert(SingleLinkedList* list, size_t idx, const void* data);

// Removes node, sets data pointed to by node to data
// If data is NULL data at node is freed
SingleLinkedListStatus single_list_pop(SingleLinkedList* list, void** data);

// Removes element at given index, sets pointer to data.
// If data is NULL node data is freed
SingleLinkedListStatus single_list_remove(SingleLinkedList* list, size_t idx, void** data);

// Clears list and frees all nodes, doesn't free list
void single_list_clear(SingleLinkedList* list);

// Reverses list in place
SingleLinkedListStatus single_list_reverse(SingleLinkedList* list);

// Sorts list in ascending order
SingleLinkedListStatus single_list_sort_asc(SingleLinkedList* list);

// Sorts list in descending order
SingleLinkedListStatus single_list_sort_desc(SingleLinkedList* list);

// Returns message corresponding to error code
const char* single_list_error(SingleLinkedListStatus error_code);

#endif