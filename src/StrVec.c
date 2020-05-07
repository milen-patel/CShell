#include "StrVec.h"

StrVec StrVec_value(size_t capacity) {
	return Vec_value(capacity, sizeof(Str));
}

void StrVec_drop(StrVec *self) {
	/* Step 1: Free all the Str values */
	while(StrVec_length(self) != 0) {
		Str toDrop = StrVec_pop(self);
		Str_drop(&toDrop);
	}
	/* Step 2: Drop self */
	Vec_drop(self);
}

size_t StrVec_length(const StrVec *self) {
	return Vec_length(self);
}

Str* StrVec_ref(const StrVec *self, size_t index) {
	return (Str*)(Vec_ref(self, index));
}

void StrVec_push(StrVec *self, Str value) {
	Vec_set(self, Vec_length(self), &value);
}

Str StrVec_pop(StrVec *self) {
	Str poppedStr = *((Str*) (Vec_ref(self, Vec_length(self)-1)));
	Vec_splice(self, Vec_length(self)-1, 1, NULL, 0);
	return poppedStr;
}

void StrVec_set(StrVec *self, size_t index, Str value) {
	if (index == Vec_length(self)) {
		Vec_splice(self, Vec_length(self), 0, &value, 1);	
	} else { 
		Str oldStr = *((Str*) (Vec_ref(self, index)));
		Vec_set(self, index, &value);
		Str_drop(&oldStr);	
	}
}
