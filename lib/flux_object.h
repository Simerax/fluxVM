#ifndef FLUX_OBJECT_H
#define FLUX_OBJECT_H

#include<stddef.h> // size_t
#include<stdbool.h> // bool


typedef enum {
    Integer,
    Double,

} flux_object_type;

typedef struct {
    unsigned int ref_count;
    flux_object_type type;
    void* value;
    size_t value_size;
} flux_object;


flux_object* flux_object_init();
void flux_object_free(flux_object*);
void flux_object_inc_ref(flux_object*);
void flux_object_dec_ref(flux_object*);


flux_object* flux_object_iinit(int);

void flux_object_print(flux_object*);

bool flux_object_itod(flux_object*);
flux_object* flux_object_copy(flux_object*);

int flux_object_get_int_value(flux_object*);
double flux_object_get_double_value(flux_object*);


flux_object_type flux_object_get_type(flux_object*);

#endif //FLUX_OBJECT_H
