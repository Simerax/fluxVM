#include "flux_list.h"
#include<stdlib.h> // malloc
#include<string.h> // memcpy
#include "flux_log.h"


flux_list* flux_list_init(size_t element_size) {
    flux_list* l = malloc(sizeof(flux_list));
    l->element_size = element_size;
    l->next = NULL;
    l->element = NULL;
    return l;
}

void flux_list_free(flux_list* l) {
    if(l == NULL)
        return;

    // TODO: FIXME: This is really bad since it does one recursion per list element has to be fixed
    if(l->next != NULL)
        flux_list_free(l->next);

    free(l);
}


void flux_list_add(flux_list* l, void* element) {
    flux_list* current_element = l;

    while(current_element->element != NULL && current_element->next != NULL) {
        current_element = current_element->next;
    }

    if(current_element->element == NULL) {
        current_element->element = element;
    } else {
        if(current_element->next == NULL) {
            current_element->next = flux_list_init(current_element->element_size);
            current_element->next->element = element;
        }
    }
}


void** flux_list_to_array(flux_list* l) {
    size_t length = flux_list_length(l);
    void** elements = malloc(l->element_size * length);

    flux_list* current_element = l;
    int i = 0;
    while(current_element != NULL && current_element->element!= NULL) {
        *(elements + (i * current_element->element_size))  = current_element->element;
        elements[i] = current_element->element;
        ++i;
        current_element = current_element->next;
    }
    return elements;
}


size_t flux_list_length(flux_list* l) {
    if(l == NULL || l->element == NULL)
        return 0;

    size_t length = 0;
    flux_list* current_element = l;
    while(current_element != NULL && current_element->element != NULL) {
        length++;
        current_element = current_element->next;
    }
    return length;
}
