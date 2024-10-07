#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "single_list.h"

//-------------------------------STATICS----------------------------------------------

static void s_free_node(Node* node) {
    free(node->data);
    free(node);
}

static void s_free_all_nodes(SingleLinkedList* list) {
    Node* curr = list->head;
    while (curr->next) {
        Node* to_free = curr;
        curr = curr->next;
        s_free_node(to_free);
    }
    s_free_node(curr);
}

//----------------------------INITIALIZATION-AND-DELETION----------------------------------------

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
    new->compar = NULL;
    new->size = 0;
    new->data_size = data_size;
    return new;
}

void single_list_free(SingleLinkedList* list) {
    if (!list->head) {
        free(list);
        return;
    }

    s_free_all_nodes(list);
    free(list);
}

//-----------------------------------GETTING-ELEMENTS--------------------------------------------------

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

SingleLinkedListStatus single_list_max(const SingleLinkedList* list, void** data) {
    if (!list->compar) {
        return COMPARATOR_FUNCTION_MISSING;
    }
    if (!data) {
        return NULL_DATA_POINTER;
    }
    if (list->size == 0) {
        return LIST_EMPTY;
    }

    void* max_data = list->head->data;
    Node* curr = list->head;
    while (curr) {
        if (list->compar(curr->data, max_data) > 0) {
            max_data = curr->data;
        }
        curr = curr->next;
    }
    *data = max_data;
    return SUCCESS;
}

SingleLinkedListStatus single_list_min(const SingleLinkedList* list, void** data) {
    if (!list->compar) {
        return COMPARATOR_FUNCTION_MISSING;
    }
    if (!data) {
        return NULL_DATA_POINTER;
    }
    if (list->size == 0) {
        return LIST_EMPTY;
    }

    void* min_data = list->head->data;
    Node* curr = list->head;
    while (curr) {
        if (list->compar(curr->data, min_data) < 0) {
            min_data = curr->data;
        }
        curr = curr->next;
    }
    *data = min_data;
    return SUCCESS;
}

//--------------------ADDING-ELEMENTS-------------------------------------------

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

//-------------------------REMOVING-ELEMENTS---------------------------------

SingleLinkedListStatus single_list_pop(SingleLinkedList* list, void** data) {
    if (list->size == 0) {
        return LIST_EMPTY;
    }

    if (list->size == 1) {
        if (data) {
            *data = list->head->data;
            free(list->head);
        } else {
            s_free_node(list->head);
        }
        list->head = list->tail = NULL;
    } else {
        Node* curr = list->head;
        while (curr->next != list->tail) {
            curr = curr->next;
        }
        Node* to_free = curr->next;
        curr->next = NULL;
        list->tail = curr;

        if (data) {
            *data = to_free->data;
            free(to_free);        
        } else {
            s_free_node(to_free);
        }
    }
    list->data_size--;
    return SUCCESS;
}

SingleLinkedListStatus single_list_remove(SingleLinkedList* list, size_t idx, void** data) {
    if (list->size == 0) {
        return LIST_EMPTY;
    }
    if (idx >= list->size) {
        return LIST_INDEX_OUT_OF_BOUNDS;
    }

    if (idx == list->size - 1 || list->size == 1) {
        return single_list_pop(list, data);
    }
    
    if (idx == 0) {
        Node* to_free = list->head;
        list->head = list->head->next;
        
        if (data) {
            *data = to_free->data;
            free(to_free);
        } else {
            s_free_node(to_free);
        }
    } else {
        Node* prev = list->head;
        Node* curr = prev->next;
        size_t i = 1;
        while (i < idx) {
            prev = curr;
            curr = curr->next;
            i++;
        }
        prev->next = curr->next;

        if (data) {
            *data = curr->data;
            free(curr);
        } else {
            s_free_node(curr);
        }
    }
    list->size--;
    return SUCCESS;
}

void single_list_clear(SingleLinkedList* list) {
    if (!list->head) {
        return;
    }

    s_free_all_nodes(list);
    list->size = 0;
    list->head = list->tail = NULL;
}

//-----------------------MODIFY------------------------------------

SingleLinkedListStatus single_list_reverse(SingleLinkedList* list) {
    if (list->size == 0) {
        return LIST_EMPTY;
    }
    if (list->size == 1) {
        return SUCCESS;
    }

    Node* curr = list->head;
    Node* prev = NULL;
    Node* next = NULL;

    list->tail = curr;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
    return SUCCESS;
}


#define BLOCKSIZE 32

static void swap_data(void** a, void** b) {
    void* temp = *a;
    *a = *b;
    *b = temp;
}

// order 1 means ascending sort, -1 means descending sort
// Function sorts each block using bubble sort
static void sort_block(Node* block, size_t block_size, Comparator compar, int8_t order) {
    for (size_t i = 0; i < block_size - 1; i++) {
        bool swapped = false;
        Node* curr = block;
        Node* next = curr->next;
        for (size_t j = 0; j < block_size - i; j++) {
            if (order * compar(curr->data, next->data) > 0) {
                swap_data(&curr->data, &next->data);
                swapped = true;
            }
            curr = next;
            next = curr->next;
        }
        if (!swapped) {
            return;
        }
    }
}

static Node* merge(Node* A, Node* B, Comparator compar, int8_t order) {
    Node* start = NULL;
    Node* curr = NULL;

    if (order * compar(A->data, B->data) <= 0) {
        start = A;
        A = A->next;
    } else {
        start = B;
        B = B->next;
    }

    curr = start;
    while (A && B) {
        if (order * compar(A->data, B->data) <= 0) {
            curr->next = A;
            A = A->next;
        } else {
            curr->next = B;
            B = B->next;
        }
        curr = curr->next;
    }

    // Only one of these loops executes
    while (A) {
        curr->next = A;
        A = A->next;
        curr = curr->next;
    }
    while (B) {
        curr->next = B;
        B = B->next;
        curr = curr->next;
    }
    curr->next = NULL;
    return start;
}

static Node* merge_blocks(Node** blocks, size_t n_blocks, Comparator compar, int8_t order) {
    bool merged = false;
    do {
        size_t block_idx = 0;
        while (true) {
            while (block_idx < n_blocks && !blocks[block_idx]) {
                block_idx++;
            }
            size_t first_idx = block_idx++;
            while (block_idx < n_blocks && !blocks[block_idx]) {
                block_idx++;
            }
            if (block_idx < n_blocks) {
                size_t second_idx = block_idx++;
                blocks[first_idx] = merge(blocks[first_idx], blocks[second_idx], compar, order);
                blocks[second_idx] = NULL;
                merged = true;
            } else {
                break;
            }
        }
    } while (merged);
    return blocks[0];
}

// positive order means asc, negative means desc according to
// comparator like used in qsort
static SingleLinkedListStatus sort(SingleLinkedList* list, int8_t order) {
    // split list to ceil(size / BLOCKSIZE) blocks
    size_t n_blocks = list->size / BLOCKSIZE + 1;
    Node** blocks = (Node**)malloc((list->size / BLOCKSIZE + 1) * sizeof(Node*));
    if (!blocks) {
        return MEMORY_ALLOCATION_ERROR;
    }

    Node* curr = list->head;
    for (size_t i = 0; i < n_blocks - 1; i++) {
        blocks[i] = curr;
        for (size_t j = 0; j < BLOCKSIZE - 1; j++) {
            curr = curr->next;
        }
        Node* temp = curr->next;
        curr->next = NULL;
        curr = temp;
    }
    blocks[n_blocks - 1] = curr;

    // Sort each block with bubble sort
    for (size_t i = 0; i < n_blocks - 1; i++) {
        sort_block(blocks[i], BLOCKSIZE, list->compar, order);
    }
    sort_block(blocks[n_blocks - 1], list->size - (n_blocks - 1) * BLOCKSIZE, list->compar, order);

    list->head = merge_blocks(blocks, n_blocks, list->compar, order);
    curr = list->head;
    while (curr) {
        curr = curr->next;
    }
    list->tail = curr;

    free(blocks);
    return SUCCESS;
}

SingleLinkedListStatus single_list_sort_asc(SingleLinkedList* list) {
    if (!list->compar) {
        return COMPARATOR_FUNCTION_MISSING;
    }
    if (list->size == 0) {
        return LIST_EMPTY;
    }
    if (list->size == 1) {
        return SUCCESS;
    }
    return sort(list, 1);
}

SingleLinkedListStatus single_list_sort_desc(SingleLinkedList* list) {
    if (!list->compar) {
        return COMPARATOR_FUNCTION_MISSING;
    }
    if (list->size == 0) {
        return LIST_EMPTY;
    }
    if (list->size == 1) {
        return SUCCESS;
    }
    return sort(list, -1);
}