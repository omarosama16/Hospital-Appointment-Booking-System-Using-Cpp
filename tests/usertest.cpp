#include <gtest/gtest.h>
#include "user.h"

TEST(UserTest, DefaultConstructor)
{
    User u;

    EXPECT_EQ(u.get_id(), 0);
    EXPECT_EQ(u.get_name(), "");
    EXPECT_EQ(u.get_email(), "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(), "");
}

TEST(UserTest, ConstructorAndGetters)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_EQ(u.get_id(), 1);
    EXPECT_EQ(u.get_name(), "Omar");
    EXPECT_EQ(u.get_email(), "omar@test.com");
    EXPECT_EQ(u.get_password(), "1234");
    EXPECT_EQ(u.get_role(), "patient");
}

TEST(UserTest, AuthenticateSuccess)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
}

TEST(UserTest, AuthenticateFail)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
}