#ifndef STR_VEC_H
#define STR_VEC_H

#include "Vec.h"
#include "Str.h"

/* StrVec is a Vector of Str values. */
typedef Vec StrVec;

/* Construct an empty StrVec value. Owner is 
 * responsible for calling Str_drop when its 
 * lifetime expires.
 */
StrVec StrVec_value(size_t capacity);

/* Owner of StrVec must call drop to expire
 * its lifetime and the heap memory its Str
 * items own. */
void StrVec_drop(StrVec *self);

/* Returns the # of Strs in the StrVec */
size_t StrVec_length(const StrVec *self);

/* Returns a pointer to a Str at given index.  */
Str* StrVec_ref(const StrVec *self, size_t index);

/* Push a Str value onto the end of the StrVec.
 * The StrVec self becomes the owner of Str value. */
void StrVec_push(StrVec *self, Str value);

/* Pop a Str value off the end of the StrVec.
 * After popping, StrVec's length is reduced by one.
 * The popped Str is returned and becomes owned
 * by the caller of StrVec_pop. */
Str StrVec_pop(StrVec *self);

/* Set a Str at a specific index. If the index
 * is the length of the StrVec, value will be appended.
 * Otherwise, whatever Str was previously at the
 * index will be dropped and then replaced with value.
 * The StrVec self becomes the owner of Str value. */
void StrVec_set(StrVec *self, size_t index, Str value);

#endif
