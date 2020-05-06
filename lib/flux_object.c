#include"flux_object.h"
#include"flux_log.h"
#include<stdlib.h>

flux_object* flux_object_init() {
    flux_object* obj = malloc(sizeof(flux_object));
    FLUX_DLOG("Initializing flux_object %p", obj);
    obj->ref_count = 0;
    flux_object_inc_ref(obj);
    return obj;
}

flux_object* flux_object_iinit(int value) {
    flux_object* obj = flux_object_init();
    obj->type = Integer;
    obj->value = malloc(sizeof(int));
    *((int*)obj->value) = value;
    return obj;
}

void flux_object_inc_ref(flux_object* obj) {
    obj->ref_count++;
    FLUX_DLOG("Increment Ref count of flux_object %p to %d", obj, obj->ref_count);
}

void flux_object_dec_ref(flux_object* obj) {
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
    if(obj == NULL)
        return;

    if(obj->type == Integer) {
        printf("%d\n", *((int*)obj->value));
    }
}
