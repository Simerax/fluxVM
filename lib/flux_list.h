#ifndef FLUX_LIST_H
#define FLUX_LIST_H
#include<stddef.h>

typedef struct FluxList {
    size_t element_size;
    void* element;
    struct FluxList* next;
} FluxList;

FluxList* flux_list_init(size_t element_size);
void flux_list_free(FluxList*);

void flux_list_add(FluxList*, void* element);

void** flux_list_to_array(FluxList*);

size_t flux_list_length(FluxList*);


#endif //FLUX_LIST_H
