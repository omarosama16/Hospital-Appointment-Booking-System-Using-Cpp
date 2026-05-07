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

TEST(UserTest, FullConstructor)
{
    User u(10, "Ali", "a@a.com", "123", "patient");

    EXPECT_EQ(u.get_id(), 10);
    EXPECT_EQ(u.get_name(), "Ali");
    EXPECT_EQ(u.get_email(), "a@a.com");
    EXPECT_EQ(u.get_password(), "123");
    EXPECT_EQ(u.get_role(), "patient");
}

TEST(UserTest, SettersCoverage)
{
    User u;

    u.set_id(5);
    u.set_name("Test");
    u.set_email("t@t.com");
    u.set_password("pass");
    u.set_role("doctor");

    EXPECT_EQ(u.get_id(), 5);
    EXPECT_EQ(u.get_name(), "Test");
    EXPECT_EQ(u.get_email(), "t@t.com");
    EXPECT_EQ(u.get_password(), "pass");
    EXPECT_EQ(u.get_role(), "doctor");
}

TEST(UserTest, AuthenticateAllBranches)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
    EXPECT_FALSE(u.Authenticate("", ""));
}