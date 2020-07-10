#include "flux_list.h"
#include<stdlib.h> // malloc
#include<string.h> // memcpy
#include "flux_log.h"


FluxList* flux_list_init(size_t element_size, void (*free_func)(void*)) {
    FluxList* l = malloc(sizeof(FluxList));
    l->element_size = element_size;
    l->next = NULL;
    l->element = NULL;
    l->element_free_func = free_func;
    return l;
}

void flux_list_free(FluxList* l) {
    if(l == NULL)
        return;

    FluxList* current = l;

    while(current != NULL) {
        FluxList* previous = current;
        current = current->next;
        if(previous->element_free_func != NULL) {
            previous->element_free_func(previous->element);
        }
        free(previous);
    }
}


void flux_list_add(FluxList* l, void* element) {
    FluxList* current_element = l;

    while(current_element->element != NULL && current_element->next != NULL) {
        current_element = current_element->next;
    }

    if(current_element->element == NULL) {
        current_element->element = element;
    } else {
        if(current_element->next == NULL) {
            current_element->next = flux_list_init(current_element->element_size, current_element->element_free_func);
            current_element->next->element = element;
        }
    }
}


void** flux_list_to_array(FluxList* l) {
    size_t length = flux_list_length(l);
    void** elements = malloc(l->element_size * length);

    FluxList* current_element = l;
    int i = 0;
    while(current_element != NULL && current_element->element!= NULL) {
        elements[i] = current_element->element;
        ++i;
        current_element = current_element->next;
    }
    return elements;
}


size_t flux_list_length(FluxList* l) {
    if(l == NULL || l->element == NULL)
        return 0;

    size_t length = 0;
    FluxList* current_element = l;
    while(current_element != NULL && current_element->element != NULL) {
        length++;
        current_element = current_element->next;
    }
    return length;
}
