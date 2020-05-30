#include"flux_object.h"
#include "flux_cmp_result.h"
#include "flux_exception.h"
#include"flux_log.h"
#include "flux_vm.h"
#include<stdlib.h>
#include<string.h> // memcpy
#include<stdio.h> // printf

FluxObject* flux_object_init() {
    FluxObject* obj = malloc(sizeof(FluxObject));
    FLUX_DLOG("Initializing FluxObject %p", obj);
    obj->ref_count = 0;
    obj->value_size = 0;
    flux_object_inc_ref(obj);
    return obj;
}

FluxObject* flux_object_strinit(char* string, unsigned int length) {
    FluxObject* obj = flux_object_init();
    obj->type = String;
    obj->value_size = length;
    obj->value = malloc(obj->value_size);
    // TODO: for now we copy the string maybe we should add a way to just move the string instead of copying
    memcpy(obj->value, string, length);
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

FluxObject* flux_object_cinit(char value) {
    FluxObject* obj = flux_object_init();
    obj->type = Char;
    obj->value_size = sizeof(char);
    obj->value = malloc(obj->value_size);
    *((char*)obj->value) = value;
    return obj;
}



size_t flux_object_get_size(FluxObject* obj) {
    return obj->value_size;
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

void flux_object_inspect(FluxObject* obj) {
    flux_object_print(obj);
}

void flux_object_print(FluxObject* obj) {
    if(obj == NULL) {
        FLUX_ELOG("Tried printing NULL Object");
        return;
    }

    switch(obj->type) {
        case Integer: printf("%d", *((int*)obj->value));
                      break;
        case Double:  printf("%f", *((double*)obj->value));
                      break;
        case Char:    printf("%c", *((char*)obj->value));
                      break;
        case String:  printf("%s", (char*)obj->value);
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

char flux_object_get_char_value(FluxObject* obj) {
    return *((char*)obj->value);
}

char* flux_object_get_str_value(FluxObject* obj) {
    return ((char*)obj->value);
}

FluxObjectType flux_object_get_type(FluxObject* obj) {
    return obj->type;
}

FluxCmpResult flux_object_cmp(FluxObject* a, FluxObject* b) {

    if(a == NULL || b == NULL)
        return NONE;

    if(flux_object_get_type(a) == flux_object_get_type(b)) {
        FluxObjectType type = flux_object_get_type(a);
        if(type == Integer) {

            int result = flux_object_get_int_value(a) - flux_object_get_int_value(b);
            if(result < 0) {
                return LESS;
            } else if (result == 0) {
                return EQUAL;
            } else {
                return GREATER;
            }
        } else if (type == Char) {
            char result = flux_object_get_char_value(a) - flux_object_get_char_value(b);
            if(result < 0) {
                return LESS;
            } else if(result == 0) {
                return EQUAL;
            } else {
                return GREATER;
            }
        }
    }
    return EQUAL; // FIXME: Well this is just wrong but rn its not a problem
}

FluxExceptionType flux_object_get_exception_type(FluxObject* obj) {
    switch(flux_object_get_type(obj)) {
        case Integer: return flux_exception_type_integer;
        case Double: return flux_exception_type_double;
        case Char: return flux_exception_type_char;
        case String: return flux_exception_type_string;
        default: return flux_exception_type_unknown;
    }
}
