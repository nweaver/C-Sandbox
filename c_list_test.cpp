#include <gtest/gtest.h>
#include <vector>

extern "C"
{
#include "c_list.h"
#include <string.h>

    bool is_foo(void *data)
    {
        return !strcmp((const char *)data, "foo");
    }

    bool is_false(void *data)
    {
        (void)data;
        return false;
    }
}

TEST(C_LIST, BasicTestsPrepend)
{
    linked_list *l = new_linked_list();
    std::vector<const char *> teststrings = {"foo", "bar", "baz", "garplay"};
    char *tmp = (char *) malloc(100);
    sprintf(tmp, "foo");

    size_t i;
    i = 0;
    for (const char *s : teststrings)
    {
        EXPECT_EQ(get_at(l, i), nullptr);
        prepend(l, (void *)s);
        i++;
        EXPECT_EQ(get_at(l, 0), s);
        EXPECT_EQ(l->length, i);
    }
    for (size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ((const char *)get_at(l, i), teststrings[3 - i]);
    }
    EXPECT_EQ(find_at(l, is_foo), 3);
    EXPECT_EQ(find_at(l, is_false), -1);
    // This does a pointer comparison
    EXPECT_NE(get_at(l, 3), tmp);
    EXPECT_TRUE(!strcmp((const char *) get_at(l, 3), tmp));
    free_linked_list(l);
    free(tmp);
};

TEST(C_LIST, BasicTestsAppend)
{
    linked_list *l = new_linked_list();
    std::vector<const char *> teststrings = {"foo", "bar", "baz", "garplay"};
    size_t i;
    i = 0;
    for (const char *s : teststrings)
    {
        EXPECT_EQ(get_at(l, i), nullptr);
        append(l, (void *)s);
        EXPECT_EQ(get_at(l, i), s);
        i++;
        EXPECT_EQ(l->length, i);
    }
    for (size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ((const char *)get_at(l, i), teststrings[i]);
    }
    free_linked_list(l);
};

TEST(C_LIST, SandboxPrint)
{
    float f = 1.0;

    printf("%i\n", *((int *)&f));
    printf("%i\n\n", (int)f);
    f = 0.0;
    printf("%i\n", *((int *)&f));
    printf("%i\n\n", (int)f);
    f = -0.0;
    printf("%i\n", *((int *)&f));
    printf("%i\n\n", (int)f);
}