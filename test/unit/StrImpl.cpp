#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}

TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}

/**
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    /* Ensure that Str_ref works for all elements of a Str */
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

TEST(StrImpl, from) {
    /* Test Str_from */
    Str s = Str_from("hello, world");
    ASSERT_EQ(s.length, 13);
    char hworld[] = "hello, world";
    for (size_t i = 0; i<s.length; i++) {
        ASSERT_EQ(*((char*)(s.buffer)+i), hworld[i]);
    }
    ASSERT_STREQ((char*)s.buffer, hworld);
    Str_drop(&s);
}

TEST(StrImpl, from_empty_str) {
    /* Test that Str_from works with an empty string */
    Str s = Str_from("");
    ASSERT_EQ(s.length, 1);
    ASSERT_EQ('\0', *(char*)s.buffer);
    Str_drop(&s);
}

TEST(StrImpl, splice_join_two_strings){
    /* Use Str_splice to join two strings */
    Str a = Str_from("hello, ");
    Str b = Str_from("world!");
    Str_splice(&a, 7, 0, (char*)b.buffer, 6); 
    ASSERT_EQ(a.length, 14);
    ASSERT_EQ(*((char*)a.buffer + 5), ',');
    ASSERT_EQ(*((char*)a.buffer + 7), 'w');
    ASSERT_EQ(*((char*)a.buffer + 14), '\0');
    ASSERT_STREQ((char*)a.buffer, "hello, world!");
    Str_drop(&a);
    Str_drop(&b);
}

TEST(StrImpl, splice_append_to_empty_string){
    /* Use Str_splice to add to an empty string */
    Str a = Str_from("");
    Str b = Str_from("world!");
    Str_splice(&a, 0, 0, (char*)b.buffer, 6); 
    ASSERT_EQ(a.length, 7);
    ASSERT_STREQ((char*)a.buffer, "world!");
    Str_drop(&a);
    Str_drop(&b);
}

TEST(StrImpl, splice_delete_null_char_attempt) {
    /* Ensure that Str_splice doesn't allow deletion of null character */
    Str a = Str_from("hello, ");
    Str b = Str_from("world!");
    Str_splice(&a, 0, 7, (char*)b.buffer, 0); 
    ASSERT_EQ(a.length, 1);
    ASSERT_DEATH({
        Str_splice(&a, 0, 1, (char*)b.buffer, 0);
    }, ".* - Out of Bounds");
    Str_drop(&a);
    Str_drop(&b);
}

TEST(StrImpl, splice_delete_only){
    /* Use Str_splice to delete from a string */
    Str a = Str_from("hellograder");
    Str b = Str_from("");
    Str_splice(&a, 3, 2, (char*)b.buffer, 0); 
    ASSERT_EQ(a.length, 10);
    ASSERT_STREQ((char*)a.buffer, "helgrader");
    Str_drop(&a);
    Str_drop(&b);
}

TEST(StrImpl, splice_insert_only){
    /* Use Str_splice to insert into a string */
    Str a = Str_from("herader");
    Str b = Str_from("llog");
    Str_splice(&a, 2, 0, (char*)b.buffer, 4); 
    ASSERT_EQ(a.length, 12);
    ASSERT_STREQ((char*)a.buffer, "hellograder");
    Str_drop(&a);
    Str_drop(&b);
}

TEST(StrImpl, splice_insert_and_delete){
    /* Use Str_splice to insert and delete from a Str */
    Str a = Str_from("heXXXrader");
    Str b = Str_from("llog");
    Str_splice(&a, 2, 3, (char*)b.buffer, 4); 
    ASSERT_EQ(a.length, 12);
    ASSERT_STREQ((char*)a.buffer, "hellograder");
    Str_drop(&a);
    Str_drop(&b);
}


TEST(StrImpl, append) {
    /* Test Str_append */
    Str a = Str_from("milen");
    char x[] = " was here!";
    Str_append(&a, x);
    ASSERT_EQ(a.length, 16);
    ASSERT_STREQ((char*)a.buffer, "milen was here!");
    Str_drop(&a);
}

TEST(StrImpl, append_to_empty_string) {
    /* Use Str_append to add to an empty string */
    Str a = Str_from("");
    char x[] = "milen";
    Str_append(&a, x);
    ASSERT_EQ(a.length, 6);
    ASSERT_STREQ((char*)a.buffer, "milen");
    Str_drop(&a);
}

TEST(StrImpl, append_an_empty_string) {
    /* Ensure that Str_append functions normally with useless input */
    Str a = Str_from("milen");
    char x[] = "";
    Str_append(&a, x);
    ASSERT_EQ(a.length, 6);
    ASSERT_STREQ((char*)a.buffer, "milen");
    Str_drop(&a);
}

TEST(StrImpl, append_grow_allocation) {
    /* Ensure that Str_append grows allocation when needed */
    Str a = Str_value(0);
    ASSERT_EQ(a.length, 1);
    ASSERT_EQ(a.capacity, 1); 
    char x[] = "four";
    Str_append(&a, x);
    ASSERT_EQ(a.length, 5);
    Str_drop(&a); 
}

TEST(StrImpl, get) {
    /* Test Str_get on all characters of a string */
    Str a = Str_from("milen");
    ASSERT_EQ(Str_get(&a,0), 'm');
    ASSERT_EQ(Str_get(&a,1), 'i');
    ASSERT_EQ(Str_get(&a,2), 'l');
    ASSERT_EQ(Str_get(&a,3), 'e');
    ASSERT_EQ(Str_get(&a,4), 'n');
    Str_drop(&a);
}

TEST(StrImpl, get_empty_str) {
    /* Ensure Str_get doesn't allow access of the null character */
    Str a = Str_from("");
    ASSERT_DEATH({
        Str_get(&a, 0);
    }, ".* - Out of Bounds");
    Str_drop(&a);
}

TEST(StrImpl, get_out_of_bounds){
    /* Ensure Str_get fails on out of bounds index */
    Str a = Str_from("milen");
    ASSERT_DEATH({
        Str_get(&a, 10);
    }, ".* - Out of Bounds");
    Str_drop(&a);
}

TEST(StrImpl, set) { 
    /* Test Str_set on all indices of a str */
    Str a = Str_from("milen");
    Str_set(&a, 0, 'M');
    Str_set(&a, 1, 'I');
    Str_set(&a, 2, 'L');
    Str_set(&a, 3, 'E');
    Str_set(&a, 4, 'N');
    ASSERT_EQ(a.length, 6);
    ASSERT_STREQ((char*)a.buffer, "MILEN");
    Str_drop(&a);
}

TEST(StrImpl, set_append){
    /* Use Str_set to append to a string */
    Str a = Str_from("milen");
    ASSERT_EQ(a.length, 6);
    Str_set(&a, 5, '!');
    ASSERT_EQ(a.length, 7);
    ASSERT_STREQ((char*)a.buffer, "milen!");
    Str_drop(&a);
}

TEST(StrImpl, set_out_of_bounds){
    /* Ensure Str_set fails on bad indices */
    Str a = Str_from("milen");
    ASSERT_DEATH({
       Str_set(&a,6,'r');
    }, ".* - Out of Bounds");
    Str_drop(&a);
}
