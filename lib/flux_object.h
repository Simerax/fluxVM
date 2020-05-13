#ifndef FLUX_OBJECT_H
#define FLUX_OBJECT_H

#include<stddef.h> // size_t
#include<stdbool.h> // bool


typedef enum {
    Integer,
    Double,

} FluxObjectType;

typedef struct {
    unsigned int ref_count;
    FluxObjectType type;
    void* value;
    size_t value_size;
} FluxObject;


FluxObject* flux_object_init();
void flux_object_free(FluxObject*);
void flux_object_inc_ref(FluxObject*);
void flux_object_dec_ref(FluxObject*);


FluxObject* flux_object_iinit(int);

void flux_object_print(FluxObject*);

bool flux_object_itod(FluxObject*);
FluxObject* flux_object_copy(FluxObject*);

int flux_object_get_int_value(FluxObject*);
double flux_object_get_double_value(FluxObject*);


FluxObjectType flux_object_get_type(FluxObject*);

#endif //FLUX_OBJECT_H
