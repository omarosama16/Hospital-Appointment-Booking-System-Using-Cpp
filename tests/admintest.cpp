#include <gtest/gtest.h>
#include "admin.h"

TEST(AdminTest, Constructor)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_EQ(a.get_id(), 1);
    EXPECT_EQ(a.get_name(), "Omar");
    EXPECT_EQ(a.get_email(), "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");
    EXPECT_EQ(a.get_role(), "admin");
}

TEST(AdminTest, AuthenticateInheritsUser)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
    EXPECT_FALSE(a.Authenticate("wrong", "1234"));
}