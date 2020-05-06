#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

/* Constructor / Destructor */

Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Vec_get(const Vec *self, size_t index, void *out) {

}

void Vec_set(Vec *self, size_t index, const void *value){

}

bool Vec_equals(const Vec *self, const Vec *other) {
	
}

void Vec_splice(
        Vec *self, 
        size_t index, 
        size_t delete_count, 
        const void *items, 
        size_t insert_count
        ) {

}

