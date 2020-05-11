#ifndef FLUX_LIST_H
#define FLUX_LIST_H
#include<stddef.h>

typedef struct flux_list {
    size_t element_size;
    void* element;
    struct flux_list* next;
} flux_list;

flux_list* flux_list_init(size_t element_size);
void flux_list_free(flux_list*);

void flux_list_add(flux_list*, void* element);

void** flux_list_to_array(flux_list*);

size_t flux_list_length(flux_list*);


#endif //FLUX_LIST_H
