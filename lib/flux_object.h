#ifndef FLUX_OBJECT_H
#define FLUX_OBJECT_H


typedef enum {
    Integer,
    Double,

} flux_object_type;

typedef struct {
    unsigned int ref_count;
    flux_object_type type;
    void* value;
} flux_object;


flux_object* flux_object_init();
void flux_object_free(flux_object*);
void flux_object_inc_ref(flux_object*);
void flux_object_dec_ref(flux_object*);


flux_object* flux_object_iinit(int);

void flux_object_print(flux_object*);

#endif //FLUX_OBJECT_H
