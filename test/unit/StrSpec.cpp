#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Str.h"
}

/**
 * The purpose of these tests is to prove correctness of the Str
 * abstraction barrier from the user's point-of-view.
 */

TEST(StrSpec, values_init_empty) {
    Str s = Str_value(10);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, values_init_cstr) {
    Str s = Str_value(10);
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);
}
