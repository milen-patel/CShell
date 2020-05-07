#ifndef CHAR_ITR_H
#define CHAR_ITR_H

#include <stdlib.h>
#include <stdbool.h>

#include "Str.h"

/* 
 * A CharItr is an iterator over an array of char data.
 */

typedef struct CharItr {
    const char *cursor;
    const char *sentinel;
} CharItr;

/*
 * Constructor. Resulting CharItr value does not own any
 * heap memory thus there is no drop function. Resulting
 * value's lifetime is dependent upon the start and sentinal
 * references.
 */
CharItr CharItr_value(const char *start, size_t length);

/**
 * Get a CharItr of a Str, starting from its first char value.
 * The lifetime of the CharItr is the shorter of the Str's lifetime
 * or an any mutation of the Str.
 */
CharItr CharItr_of_Str(const Str *str);

/*
 * Returns a pointer to the current location of the iterator's cursor.
 */
const char* CharItr_cursor(const CharItr *self);

/*
 * Returns true when there are additional characters to consume
 * in the iterable range.
 */
bool CharItr_has_next(const CharItr *self);

/*
 * Peek and return the next character. Do not advance cursor.
 * Will exit with out of bounds error if no more characters
 * to consume.
 */
char CharItr_peek(const CharItr *self);

/*
 * Read next character and advance the cursor.
 * Will exit with out of bounds error if no more characters
 * to consume.
 */
char CharItr_next(CharItr *self);

#endif
