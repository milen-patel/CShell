#include <stdio.h>

#include "CharItr.h"

CharItr CharItr_value(const char *start, size_t length) {
	CharItr rnVal;
	rnVal.cursor = start;
	rnVal.sentinel = start + length;
	return rnVal;
}

CharItr CharItr_of_Str(const Str *str) {
	/* Feed data into CharItr_value */
	return CharItr_value(Str_cstr(str), Str_length(str));
}

const char* CharItr_cursor(const CharItr *self) {
	return self->cursor;
}

bool CharItr_has_next(const CharItr *self){
	return (self->cursor < self->sentinel);
}

char CharItr_peek(const CharItr *self){
	if (!CharItr_has_next(self)) {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
	} else {
		return *(self->cursor);
	}
}

char CharItr_next(CharItr *self){
	char rnVal = CharItr_peek(self);
	self->cursor++;
	return rnVal;
}
