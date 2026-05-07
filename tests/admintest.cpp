#include <gtest/gtest.h>
#include "admin.h"

TEST(AdminTest, ConstructorBasic)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_EQ(a.get_id(), 1);
    EXPECT_EQ(a.get_name(), "Omar");
    EXPECT_EQ(a.get_email(), "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");
    EXPECT_EQ(a.get_role(), "admin");
}

TEST(AdminTest, AuthenticateSuccess)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
}

TEST(AdminTest, AuthenticateWrongEmail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
}

TEST(AdminTest, AuthenticateWrongPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
}

TEST(AdminTest, AuthenticateBothWrong)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_FALSE(a.Authenticate("x", "y"));
}

TEST(AdminTest, EmptyCredentialsEdge)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_FALSE(a.Authenticate("", ""));
}

TEST(AdminTest, InheritanceRoleFixed)
{
    Admin a(10, "AdminUser", "admin@mail.com", "admin123");

    EXPECT_EQ(a.get_role(), "admin");

    a.set_role("doctor");
    EXPECT_EQ(a.get_role(), "doctor");
}