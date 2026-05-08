#include <gtest/gtest.h>
#include "admin.h"

/* ================= BASIC CONSTRUCTOR ================= */

TEST(AdminTest, ConstructorBasic)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_EQ(a.get_id(),       1);
    EXPECT_EQ(a.get_name(),     "Omar");
    EXPECT_EQ(a.get_email(),    "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");
    EXPECT_EQ(a.get_role(),     "admin");
}

// Different id value — forces id initializer condition
TEST(AdminTest, ConstructorDifferentId)
{
    Admin a(99, "Alice", "alice@test.com", "pass99");
    EXPECT_EQ(a.get_id(),       99);
    EXPECT_EQ(a.get_name(),     "Alice");
    EXPECT_EQ(a.get_email(),    "alice@test.com");
    EXPECT_EQ(a.get_password(), "pass99");
    EXPECT_EQ(a.get_role(),     "admin");
}

// Zero id — boundary for id field
TEST(AdminTest, ConstructorZeroId)
{
    Admin a(0, "Zero", "zero@test.com", "zeropass");
    EXPECT_EQ(a.get_id(),   0);
    EXPECT_EQ(a.get_role(), "admin");
}

// Negative id — another boundary
TEST(AdminTest, ConstructorNegativeId)
{
    Admin a(-1, "Neg", "neg@test.com", "negpass");
    EXPECT_EQ(a.get_id(),   -1);
    EXPECT_EQ(a.get_role(), "admin");
}

// Empty strings for all string fields
TEST(AdminTest, ConstructorEmptyStrings)
{
    Admin a(1, "", "", "");
    EXPECT_EQ(a.get_name(),     "");
    EXPECT_EQ(a.get_email(),    "");
    EXPECT_EQ(a.get_password(), "");
    EXPECT_EQ(a.get_role(),     "admin");
}

// Long strings — stress the string initializer conditions
TEST(AdminTest, ConstructorLongStrings)
{
    std::string longName(200, 'A');
    std::string longEmail(200, 'B');
    std::string longPass(200, 'C');

    Admin a(42, longName, longEmail, longPass);
    EXPECT_EQ(a.get_name(),     longName);
    EXPECT_EQ(a.get_email(),    longEmail);
    EXPECT_EQ(a.get_password(), longPass);
    EXPECT_EQ(a.get_role(),     "admin");
}

// Special characters
TEST(AdminTest, ConstructorSpecialCharacters)
{
    Admin a(5, "O'Brien", "o.brien+tag@test.com", "p@$$w0rd!");
    EXPECT_EQ(a.get_name(),     "O'Brien");
    EXPECT_EQ(a.get_email(),    "o.brien+tag@test.com");
    EXPECT_EQ(a.get_password(), "p@$$w0rd!");
    EXPECT_EQ(a.get_role(),     "admin");
}

/* ================= ROLE IS ALWAYS ADMIN ================= */

TEST(AdminTest, RoleIsAlwaysAdmin)
{
    Admin a1(1, "A", "a@test.com", "pass1");
    Admin a2(2, "B", "b@test.com", "pass2");
    Admin a3(3, "C", "c@test.com", "pass3");

    EXPECT_EQ(a1.get_role(), "admin");
    EXPECT_EQ(a2.get_role(), "admin");
    EXPECT_EQ(a3.get_role(), "admin");
}

/* ================= AUTHENTICATE - ALL CONDITION BRANCHES ================= */

// email=TRUE && password=TRUE → true
TEST(AdminTest, Authenticate_Success)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
}

// email=TRUE && password=FALSE → false
TEST(AdminTest, Authenticate_WrongPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
}

// email=FALSE (short-circuit) → false
TEST(AdminTest, Authenticate_WrongEmail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
}

// email=FALSE && password=FALSE → false
TEST(AdminTest, Authenticate_BothWrong)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("x", "y"));
}

// email=TRUE && password=FALSE: email matches but password is empty
TEST(AdminTest, Authenticate_CorrectEmailEmptyPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("omar@test.com", ""));
}

// email=FALSE: empty email with correct password
TEST(AdminTest, Authenticate_EmptyEmailCorrectPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("", "1234"));
}

// email=TRUE && password=TRUE on empty-string admin
TEST(AdminTest, Authenticate_EmptyCredentialsMatch)
{
    Admin a(1, "", "", "");
    EXPECT_TRUE(a.Authenticate("", ""));
}

// email=FALSE on empty-string admin
TEST(AdminTest, Authenticate_EmptyAdmin_WrongEmail)
{
    Admin a(1, "", "", "");
    EXPECT_FALSE(a.Authenticate("someone@test.com", ""));
}

// email=TRUE && password=FALSE on empty-string admin
TEST(AdminTest, Authenticate_EmptyAdmin_WrongPassword)
{
    Admin a(1, "", "", "");
    EXPECT_FALSE(a.Authenticate("", "wrongpass"));
}

/* ================= SETTERS ================= */

TEST(AdminTest, SettersWork)
{
    Admin a(1, "Omar", "omar@test.com", "1234");

    a.set_id(55);
    a.set_name("NewName");
    a.set_email("new@test.com");
    a.set_password("newpass");

    EXPECT_EQ(a.get_id(),       55);
    EXPECT_EQ(a.get_name(),     "NewName");
    EXPECT_EQ(a.get_email(),    "new@test.com");
    EXPECT_EQ(a.get_password(), "newpass");
    EXPECT_EQ(a.get_role(),     "admin"); // role unchanged
}

// Authenticate with new credentials after setters
TEST(AdminTest, Authenticate_AfterSetters_Success)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_TRUE(a.Authenticate("new@test.com", "newpass"));
}

TEST(AdminTest, Authenticate_AfterSetters_OldCredentialsFail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("omar@test.com", "1234")); // old creds fail
}

TEST(AdminTest, Authenticate_AfterSetters_WrongPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("new@test.com", "wrong"));
}

TEST(AdminTest, Authenticate_AfterSetters_WrongEmail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("other@test.com", "newpass"));
}

/* ================= ROLE MUTATION ================= */

TEST(AdminTest, RoleCanBeChanged)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_EQ(a.get_role(), "admin");
    a.set_role("doctor");
    EXPECT_EQ(a.get_role(), "doctor");
    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234")); // auth unaffected
}

TEST(AdminTest, RoleChangeThenBack)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_role("patient");
    EXPECT_EQ(a.get_role(), "patient");
    a.set_role("admin");
    EXPECT_EQ(a.get_role(), "admin");
}

/* ================= MULTIPLE INSTANCES ================= */

TEST(AdminTest, MultipleAdminInstances)
{
    Admin a1(1, "Admin1", "a1@test.com", "pass1");
    Admin a2(2, "Admin2", "a2@test.com", "pass2");
    Admin a3(3, "Admin3", "a3@test.com", "pass3");

    // Each authenticates independently
    EXPECT_TRUE(a1.Authenticate("a1@test.com", "pass1"));
    EXPECT_TRUE(a2.Authenticate("a2@test.com", "pass2"));
    EXPECT_TRUE(a3.Authenticate("a3@test.com", "pass3"));

    // Cross-authentication fails
    EXPECT_FALSE(a1.Authenticate("a2@test.com", "pass2"));
    EXPECT_FALSE(a2.Authenticate("a1@test.com", "pass1"));
    EXPECT_FALSE(a3.Authenticate("a1@test.com", "pass1"));
}

/* ================= STRESS ================= */

TEST(AdminTest, AuthenticateStress)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
        EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
        EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
    }
}
