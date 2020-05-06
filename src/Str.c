#include <string.h>

#include "Str.h"
#include "Vec.h"

static char NULL_CHAR = '\0';

Str Str_value(size_t capacity)
{
    Str s = Vec_value(capacity + 1, sizeof(char));
    // TODO: Replace the below lines with a call below to Vec_set
    // once you have Vec_set correctly implemented
    s.length = 1;
    char *buffer = (char*) s.buffer;
    buffer[0] = NULL_CHAR;
    // Vec_set(&s, 0, &NULL_CHAR);
    return s;
}

void Str_drop(Str *self)
{
    Vec_drop(self);
}

size_t Str_length(const Str *self)
{
    return Vec_length(self) - 1;
}

const char* Str_cstr(const Str *self)
{
    return (char*) Vec_ref(self, 0);
}

char* Str_ref(const Str *self, const size_t index)
{
    return (char*) Vec_ref(self, index);
}
