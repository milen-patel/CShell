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

TEST(StrSpec, from_constructor) {
    Str s = Str_from("hey");
    ASSERT_STREQ(Str_cstr(&s), "hey");
    ASSERT_EQ(Str_length(&s), 3);
    Str_drop(&s);
}

TEST(StrSpec, append_two_strings) {
    Str first = Str_from("hello ");
    Str second = Str_from("world");
    Str_append(&first, Str_cstr(&second));
    ASSERT_STREQ(Str_cstr(&first), "hello world");
    ASSERT_EQ(Str_length(&first), 11);
    Str_drop(&first);
    Str_drop(&second);
}

TEST(StrSpec, append_empty_string){
    /* Ensure Str_append works on empty input */
    Str first = Str_value(1);
    Str second = Str_from("hello world");
    Str_append(&first, Str_cstr(&second));
    ASSERT_STREQ(Str_cstr(&first), "hello world");
    ASSERT_EQ(Str_length(&first), 11);
    Str_drop(&first);
    Str_drop(&second);
}

TEST(StrSpec, get_all_chars) {
    /* Ensure Str_get works on all indices */
    Str s = Str_from("hi");
    ASSERT_EQ(Str_get(&s, 0), 'h');
    ASSERT_EQ(Str_get(&s, 1), 'i');
    ASSERT_DEATH({
        Str_get(&s, 2);
    }, ".* - Out of Bounds");
    Str_drop(&s);
}

TEST(StrSpec, set_general) {
    /* Ensure Str_set works on all indices */
    Str s = Str_from("hi");
    Str_set(&s, 0, 'H');
    Str_set(&s, 1, 'I');
    ASSERT_STREQ(Str_cstr(&s), "HI");
    Str_drop(&s);
}

TEST(StrSpec, set_append) {
    /* Ensure that strings can be appended to with Str_append */
    Str s = Str_from("hi");
    Str_set(&s, 2, '!');
    ASSERT_STREQ(Str_cstr(&s), "hi!");
    ASSERT_EQ(Str_length(&s), 3);
    Str_drop(&s);
}

TEST(StrSpec, splice_delete) {
    /* Use Str_splice to delete characters from a Str */
    Str s = Str_from("heXXXllo");
    Str_splice(&s, 2, 3, "", 0);
    ASSERT_STREQ(Str_cstr(&s), "hello");
    ASSERT_EQ(Str_length(&s), 5);
    Str_drop(&s);
}

TEST(StrSpec, splice_insert) {
    /* Use Str_splice to insert characters */
    Str s = Str_from("ho");
    Str_splice(&s, 1, 0, "ell", 3);
    ASSERT_STREQ(Str_cstr(&s), "hello");
    ASSERT_EQ(Str_length(&s), 5);
    Str_drop(&s);
}

TEST(StrSpec, splice_combo) {
    /* Use Str_splice to combine two Str values */
    Str s = Str_from("hwho");
    Str_splice(&s, 1, 2, "ell", 3);
    ASSERT_STREQ(Str_cstr(&s), "hello");
    ASSERT_EQ(Str_length(&s), 5);
    Str_drop(&s);
}
