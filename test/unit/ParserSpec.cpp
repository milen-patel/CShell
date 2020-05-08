#include "gtest/gtest.h"

extern "C" {
#include "Parser.h"
}

static Scanner fixture(const char *cstr)
{
    Str input = Str_from(cstr);
    return Scanner_value(CharItr_of_Str(&input));
}

TEST(ParserSpec, empty)
{
    Scanner scanner = fixture("");
    Node *ast = parse(&scanner);
    ASSERT_EQ(ERROR_NODE, ast->type);
    Node_drop(ast);
}

TEST(ParserSpec, simple_command)
{
    Scanner scanner = fixture("grep foo bar.txt");
    Node *ast = parse(&scanner);
    ASSERT_EQ(COMMAND_NODE, ast->type);
    ASSERT_STREQ("grep", Str_cstr(StrVec_ref(&ast->data.command, 0)));
    ASSERT_STREQ("foo", Str_cstr(StrVec_ref(&ast->data.command, 1)));
    ASSERT_STREQ("bar.txt", Str_cstr(StrVec_ref(&ast->data.command, 2)));
    Node_drop(ast);
}

TEST(ParserSpec, pipe_command)
{
    Scanner scanner = fixture("ls -lah | grep foo");
    Node *ast = parse(&scanner);

    ASSERT_EQ(PIPE_NODE, ast->type);

    Node *lhs = ast->data.pipe.left;
    ASSERT_EQ(COMMAND_NODE, lhs->type);
    ASSERT_STREQ("ls", Str_cstr(StrVec_ref(&lhs->data.command, 0)));
    ASSERT_STREQ("-lah", Str_cstr(StrVec_ref(&lhs->data.command, 1)));

    Node *rhs = ast->data.pipe.right;
    ASSERT_EQ(COMMAND_NODE, rhs->type);
    ASSERT_STREQ("grep", Str_cstr(StrVec_ref(&rhs->data.command, 0)));
    ASSERT_STREQ("foo", Str_cstr(StrVec_ref(&rhs->data.command, 1)));

    Node_drop(ast);
}

TEST(ParserSpec, multiple_pipe_command)
{
    Scanner scanner = fixture("ls -lah | grep -E foo | less");
    Node *ast = parse(&scanner);

    ASSERT_EQ(PIPE_NODE, ast->type);

    Node *lhs = ast->data.pipe.left;
    ASSERT_EQ(COMMAND_NODE, lhs->type);
    ASSERT_STREQ("ls", Str_cstr(StrVec_ref(&lhs->data.command, 0)));
    ASSERT_STREQ("-lah", Str_cstr(StrVec_ref(&lhs->data.command, 1)));

    Node *rhs = ast->data.pipe.right;
    ASSERT_EQ(PIPE_NODE, rhs->type);

    Node *rhs_lhs = rhs->data.pipe.left;
    ASSERT_EQ(COMMAND_NODE, rhs_lhs->type);
    ASSERT_STREQ("grep", Str_cstr(StrVec_ref(&rhs_lhs->data.command, 0)));
    ASSERT_STREQ("-E", Str_cstr(StrVec_ref(&rhs_lhs->data.command, 1)));
    ASSERT_STREQ("foo", Str_cstr(StrVec_ref(&rhs_lhs->data.command, 2)));

    Node *rhs_rhs = rhs->data.pipe.right;
    ASSERT_EQ(COMMAND_NODE, rhs_rhs->type);
    ASSERT_STREQ("less", Str_cstr(StrVec_ref(&rhs_rhs->data.command, 0)));

    Node_drop(ast);
}

