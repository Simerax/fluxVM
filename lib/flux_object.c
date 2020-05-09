#include"flux_object.h"
#include"flux_log.h"
#include<stdlib.h>
#include<string.h> // memcpy

flux_object* flux_object_init() {
    flux_object* obj = malloc(sizeof(flux_object));
    FLUX_DLOG("Initializing flux_object %p", obj);
    obj->ref_count = 0;
    obj->value_size = 0;
    flux_object_inc_ref(obj);
    return obj;
}

flux_object* flux_object_iinit(int value) {
    flux_object* obj = flux_object_init();
    obj->type = Integer;
    obj->value_size = sizeof(int);
    obj->value = malloc(obj->value_size);
    *((int*)obj->value) = value;
    return obj;
}

void flux_object_inc_ref(flux_object* obj) {
    if(obj == NULL)
        return;
    obj->ref_count++;
    FLUX_DLOG("Increment Ref count of flux_object %p to %d", obj, obj->ref_count);
}

void flux_object_dec_ref(flux_object* obj) {
    if(obj == NULL)
        return;
    obj->ref_count--;
    FLUX_DLOG("Decrement Ref count of flux_object %p to %d", obj, obj->ref_count);

    if(obj->ref_count == 0) {
        flux_object_free(obj);
    }
}

void flux_object_free(flux_object* obj) {
    if(obj == NULL)
        return;

    FLUX_DLOG("Freeing flux_object %p", obj);

    // https://stackoverflow.com/questions/2182103/is-it-ok-to-free-void
    free(obj->value);
    free(obj);
}

void flux_object_print(flux_object* obj) {
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

flux_object* flux_object_copy(flux_object* original) {
    if(original == NULL)
        return NULL;

    flux_object* copy = flux_object_init();
    copy->type = original->type;
    copy->value = malloc(sizeof(original->value_size));
    copy->value_size = original->value_size;
    memcpy(copy->value, original->value, copy->value_size);
    return copy;
}

bool flux_object_itod(flux_object* obj) {
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

int flux_object_get_int_value(flux_object* obj) {
    return *((int*)obj->value);
}

double flux_object_get_double_value(flux_object* obj) {
    return *((double*)obj->value);
}

flux_object_type flux_object_get_type(flux_object* obj) {
    return obj->type;
}
