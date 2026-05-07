#include <gtest/gtest.h>
#include "admin.h"

/* ================= BASIC ================= */

TEST(AdminTest, ConstructorBasic)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_EQ(a.get_id(), 1);
    EXPECT_EQ(a.get_name(), "Omar");
    EXPECT_EQ(a.get_email(), "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");
    EXPECT_EQ(a.get_role(), "admin");
}

/* ================= AUTH EDGE ================= */

TEST(AdminTest, AuthenticateSuccess)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
}

TEST(AdminTest, AuthenticateWrongInputs)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
    EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
    EXPECT_FALSE(a.Authenticate("x", "y"));
    EXPECT_FALSE(a.Authenticate("", ""));
}

/* ================= STATE CONSISTENCY ================= */

TEST(AdminTest, RoleIntegrityShouldNotBreakLogin)
{
    Admin a(10, "AdminUser", "admin@mail.com", "admin123");

    EXPECT_EQ(a.get_role(), "admin");

    a.set_role("doctor");
    EXPECT_EQ(a.get_role(), "doctor");

    EXPECT_TRUE(a.Authenticate("admin@mail.com", "admin123"));
}

/* ================= STRESS (repeated auth calls) ================= */

TEST(AdminTest, AuthenticateStress)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
        EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
    }
}

/* ================= BOUNDARY INPUTS ================= */

TEST(AdminTest, BoundaryInputs)
{
    Admin a(1, "", "", "");

    EXPECT_EQ(a.get_name(), "");
    EXPECT_EQ(a.get_email(), "");
    EXPECT_EQ(a.get_password(), "");

    EXPECT_TRUE(a.Authenticate("", ""));
}

/* ================= MUTATION CHECK ================= */

TEST(AdminTest, PasswordIsolation)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    a.set_name("NewName");

    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
}