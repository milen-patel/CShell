#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
}

TEST(StrGrader, empty)
{
    Str s;

    s = Str_value(0);
    ASSERT_EQ(0, Str_length(&s));
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);

    s = Str_from("");
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrGrader, from)
{
    const char *input = "hello world";
    Str str = Str_from(input);
    ASSERT_NE(input, Str_cstr(&str)); // Not same pointer addr
    ASSERT_STREQ(input, Str_cstr(&str)); // Same contents!
    ASSERT_EQ(strlen(input), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, ref)
{
    const char *input = "01234";
    Str str = Str_from(input);
    const char *start = Str_cstr(&str);
    for (size_t i = 0; i < strlen(input); ++i) {
        ASSERT_EQ(start + i, Str_ref(&str, i));    
    }
    Str_drop(&str);
}

TEST(StrGrader, splice_prepend)
{
    Str str = Str_from("abcdefg");
    const char *insert = "1234";
    Str_splice(&str, 0, 0, insert, strlen(insert));

    const char *expected = "1234abcdefg";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_append)
{
    Str str = Str_from("abcdefg");
    const char *insert = "1234";
    Str_splice(&str, 7, 0, insert, strlen(insert));

    const char *expected = "abcdefg1234";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_insert)
{
    Str str = Str_from("abcdefg");
    const char *insert = "1234";
    Str_splice(&str, 3, 0, insert, strlen(insert));

    const char *expected = "abc1234defg";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_delete)
{
    Str str = Str_from("abcdefg");
    Str_splice(&str, 3, 2, NULL, 0);

    const char *expected = "abcfg";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_same_length)
{
    Str str = Str_from("abcdefg");
    const char *insert = "123";
    Str_splice(&str, 2, 3, insert, strlen(insert));

    const char *expected = "ab123fg";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_longer_delete)
{
    Str str = Str_from("abcdefg");
    const char *insert = "123";
    Str_splice(&str, 2, 4, insert, strlen(insert));

    const char *expected = "ab123g";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, splice_replace_longer_insert)
{
    Str str = Str_from("abcdefg");
    const char *insert = "123";
    Str_splice(&str, 2, 1, insert, strlen(insert));

    const char *expected = "ab123defg";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, append)
{
    Str str = Str_from("abcdefg");
    const char *append = "123";
    Str_append(&str, append);

    const char *expected = "abcdefg123";
    ASSERT_STREQ(expected, Str_cstr(&str));
    ASSERT_EQ(strlen(expected), Str_length(&str));
    Str_drop(&str);
}

TEST(StrGrader, get)
{
    const char *input = "abcdefg";
    Str str = Str_from(input);
    for (size_t i = 0; i < Str_length(&str); ++i) {
        ASSERT_EQ(input[i], Str_get(&str, i));
    }
    Str_drop(&str);
}

TEST(StrGrader, set)
{
    const char *input = "abcdefg";
    Str str = Str_from("");
    for (size_t i = 0; i < strlen(input); ++i) {
        Str_set(&str, i, input[i]);
    }
    ASSERT_STREQ(input, Str_cstr(&str));
    Str_drop(&str);
}
