#include "gtest/gtest.h"

extern "C" {
#include "Scanner.h"
}

/** HELPER FUNCTIONS **/

static Scanner fixture(const char *cstr)
{
    Str input = Str_from(cstr);
    return Scanner_value(CharItr_of_Str(&input));
}

static Token Token_init(TokenType type, const char *cstr)
{
    Token token = {
        type,
        Str_from(cstr)
    };
    return token;
}

static void ASSERT_TOKEN_EQ(Token expect, Token actual)
{
    ASSERT_EQ(expect.type, actual.type);
    ASSERT_STREQ(Str_cstr(&expect.lexeme), Str_cstr(&actual.lexeme));
}

static void ASSERT_TOKENS_EQ(Token expected[], size_t count, Scanner s)
{
    for (size_t i = 0; i < count; ++i) {
        ASSERT_TRUE(Scanner_has_next(&s));
        Token expect = expected[i];
        ASSERT_TOKEN_EQ(expect, Scanner_peek(&s));
        Token next = Scanner_next(&s);
        ASSERT_TOKEN_EQ(expect, next);
        Str_drop(&expect.lexeme);
        Str_drop(&next.lexeme);
    }
    ASSERT_FALSE(Scanner_has_next(&s));
}

/** TESTS **/

TEST(ScannerSpec, empty_string)
{
    Scanner s = fixture("");
    ASSERT_FALSE(Scanner_has_next(&s));
    ASSERT_EQ(END_TOKEN, Scanner_peek(&s).type);
}

TEST(ScannerSpec, single_word)
{
    Scanner scanner = fixture("ls");
    Token expected[] = {
        { WORD_TOKEN, Str_from("ls") }
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, single_word_special_chars)
{
    Scanner scanner = fixture("hello-world-123");
    Token expected[] = {
        { WORD_TOKEN, Str_from("hello-world-123") }
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, single_word_with_whitespace)
{
    Scanner scanner = fixture(" \t ls  \t");
    Token expected[] = {
        { WORD_TOKEN, Str_from("ls") }
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, two_words)
{
    Scanner scanner = fixture("ls -lah");
    Token expected[] = {
        { WORD_TOKEN, Str_from("ls") },
        { WORD_TOKEN, Str_from("-lah") },
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, many_words)
{
    Scanner scanner = fixture("the quick brown fox jumped over the fence");
    Token expected[] = {
        { WORD_TOKEN, Str_from("the") },
        { WORD_TOKEN, Str_from("quick") },
        { WORD_TOKEN, Str_from("brown") },
        { WORD_TOKEN, Str_from("fox") },
        { WORD_TOKEN, Str_from("jumped") },
        { WORD_TOKEN, Str_from("over") },
        { WORD_TOKEN, Str_from("the") },
        { WORD_TOKEN, Str_from("fence") },
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, single_pipe)
{
    Scanner scanner = fixture("|");
    Token expected[] = {
        { PIPE_TOKEN, Str_from("|") },
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, whitespace_pipe)
{
    Scanner scanner = fixture(" \t | \t ");
    Token expected[] = {
        { PIPE_TOKEN, Str_from("|") },
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}

TEST(ScannerSpec, many_words_and_pipes)
{
    Scanner scanner = fixture("  \t the | \t quick \t \t brown | fox \t   | jumped   ");
    Token expected[] = {
        { WORD_TOKEN, Str_from("the") },
        { PIPE_TOKEN, Str_from("|") },
        { WORD_TOKEN, Str_from("quick") },
        { WORD_TOKEN, Str_from("brown") },
        { PIPE_TOKEN, Str_from("|") },
        { WORD_TOKEN, Str_from("fox") },
        { PIPE_TOKEN, Str_from("|") },
        { WORD_TOKEN, Str_from("jumped") },
    };
    ASSERT_TOKENS_EQ(expected, sizeof(expected) / sizeof(Token), scanner);
}
