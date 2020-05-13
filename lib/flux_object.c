#include"flux_object.h"
#include"flux_log.h"
#include<stdlib.h>
#include<string.h> // memcpy

FluxObject* flux_object_init() {
    FluxObject* obj = malloc(sizeof(FluxObject));
    FLUX_DLOG("Initializing FluxObject %p", obj);
    obj->ref_count = 0;
    obj->value_size = 0;
    flux_object_inc_ref(obj);
    return obj;
}

FluxObject* flux_object_iinit(int value) {
    FluxObject* obj = flux_object_init();
    obj->type = Integer;
    obj->value_size = sizeof(int);
    obj->value = malloc(obj->value_size);
    *((int*)obj->value) = value;
    return obj;
}

void flux_object_inc_ref(FluxObject* obj) {
    if(obj == NULL)
        return;
    obj->ref_count++;
    FLUX_DLOG("Increment Ref count of FluxObject %p to %d", obj, obj->ref_count);
}

void flux_object_dec_ref(FluxObject* obj) {
    if(obj == NULL)
        return;
    obj->ref_count--;
    FLUX_DLOG("Decrement Ref count of FluxObject %p to %d", obj, obj->ref_count);

    if(obj->ref_count == 0) {
        flux_object_free(obj);
    }
}

void flux_object_free(FluxObject* obj) {
    if(obj == NULL)
        return;

    FLUX_DLOG("Freeing FluxObject %p", obj);

    // https://stackoverflow.com/questions/2182103/is-it-ok-to-free-void
    free(obj->value);
    free(obj);
}

void flux_object_print(FluxObject* obj) {
    if(obj == NULL) {
        FLUX_ELOG("Tried printing NULL Object");
        return;
    }

    switch(obj->type) {
        case Integer: printf("%d\n", *((int*)obj->value));
                      break;
        case Double:  printf("%f\n", *((double*)obj->value));
                      break;
        default: printf("Tried printing of unsupported obj type! %p\n", obj);
    }
}

FluxObject* flux_object_copy(FluxObject* original) {
    if(original == NULL)
        return NULL;

    FluxObject* copy = flux_object_init();
    copy->type = original->type;
    copy->value = malloc(sizeof(original->value_size));
    copy->value_size = original->value_size;
    memcpy(copy->value, original->value, copy->value_size);
    return copy;
}

bool flux_object_itod(FluxObject* obj) {
    if(obj == NULL)
        return false;

    if(obj->type == Double)
        return true;

    if(obj->type == Integer) {
        int current_value = *((int*)obj->value);
        free(obj->value);
        obj->value_size = sizeof(double);
        obj->value = malloc(obj->value_size);
        *((double*)obj->value) = (double)current_value;
        obj->type = Double;
        return true;
    } else {
        return false;
    }

}

int flux_object_get_int_value(FluxObject* obj) {
    return *((int*)obj->value);
}

double flux_object_get_double_value(FluxObject* obj) {
    return *((double*)obj->value);
}

FluxObjectType flux_object_get_type(FluxObject* obj) {
    return obj->type;
}
