#include <gtest/gtest.h>
#include "admin.h"

/* ================= CONSTRUCTOR ================= */

TEST(AdminTest, Constructor_Basic)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_EQ(a.get_id(),       1);
    EXPECT_EQ(a.get_name(),     "Omar");
    EXPECT_EQ(a.get_email(),    "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");
    EXPECT_EQ(a.get_role(),     "admin");
}

TEST(AdminTest, Constructor_DifferentId)
{
    Admin a(99, "Alice", "alice@test.com", "pass99");
    EXPECT_EQ(a.get_id(),       99);
    EXPECT_EQ(a.get_name(),     "Alice");
    EXPECT_EQ(a.get_email(),    "alice@test.com");
    EXPECT_EQ(a.get_password(), "pass99");
    EXPECT_EQ(a.get_role(),     "admin");
}

TEST(AdminTest, Constructor_ZeroId)
{
    Admin a(0, "Zero", "zero@test.com", "zeropass");
    EXPECT_EQ(a.get_id(),   0);
    EXPECT_EQ(a.get_role(), "admin");
}

TEST(AdminTest, Constructor_NegativeId)
{
    Admin a(-1, "Neg", "neg@test.com", "negpass");
    EXPECT_EQ(a.get_id(),   -1);
    EXPECT_EQ(a.get_role(), "admin");
}

TEST(AdminTest, Constructor_EmptyStrings)
{
    Admin a(1, "", "", "");
    EXPECT_EQ(a.get_name(),     "");
    EXPECT_EQ(a.get_email(),    "");
    EXPECT_EQ(a.get_password(), "");
    EXPECT_EQ(a.get_role(),     "admin");
}

TEST(AdminTest, Constructor_LongStrings)
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

TEST(AdminTest, Constructor_SpecialCharacters)
{
    Admin a(5, "O'Brien", "o+tag@test.com", "p@$$w0rd!");
    EXPECT_EQ(a.get_name(),     "O'Brien");
    EXPECT_EQ(a.get_email(),    "o+tag@test.com");
    EXPECT_EQ(a.get_password(), "p@$$w0rd!");
    EXPECT_EQ(a.get_role(),     "admin");
}

/* ================= COPY / MOVE OPERATIONS - FIX FOR 1 UNCOVERED CONDITION ================= */

// Copy constructor - covers compiler-generated copy conditions
TEST(AdminTest, CopyConstructor)
{
    Admin a1(1, "Omar", "omar@test.com", "1234");
    Admin a2(a1);  // copy constructor
    EXPECT_EQ(a2.get_id(),        1);
    EXPECT_EQ(a2.get_name(),      "Omar");
    EXPECT_EQ(a2.get_email(),     "omar@test.com");
    EXPECT_EQ(a2.get_password(),  "1234");
    EXPECT_EQ(a2.get_role(),      "admin");

    // Verify independent copies
    a2.set_name("Changed");
    EXPECT_EQ(a1.get_name(), "Omar");
    EXPECT_EQ(a2.get_name(), "Changed");
}

// Copy assignment - covers compiler-generated copy assignment conditions
TEST(AdminTest, CopyAssignment)
{
    Admin a1(1, "Omar", "omar@test.com", "1234");
    Admin a2(2, "Alice", "alice@test.com", "5678");
    a2 = a1;  // copy assignment
    EXPECT_EQ(a2.get_id(),        1);
    EXPECT_EQ(a2.get_name(),      "Omar");
    EXPECT_EQ(a2.get_email(),     "omar@test.com");
    EXPECT_EQ(a2.get_password(),  "1234");
    EXPECT_EQ(a2.get_role(),      "admin");
}

// Move constructor - covers compiler-generated move conditions
TEST(AdminTest, MoveConstructor)
{
    Admin a1(1, "Omar", "omar@test.com", "1234");
    Admin a2(std::move(a1));  // move constructor
    EXPECT_EQ(a2.get_id(),       1);
    EXPECT_EQ(a2.get_name(),     "Omar");
    EXPECT_EQ(a2.get_email(),    "omar@test.com");
    EXPECT_EQ(a2.get_password(), "1234");
    EXPECT_EQ(a2.get_role(),     "admin");
}

// Move assignment - covers compiler-generated move assignment conditions
TEST(AdminTest, MoveAssignment)
{
    Admin a1(1, "Omar", "omar@test.com", "1234");
    Admin a2(2, "Alice", "alice@test.com", "5678");
    a2 = std::move(a1);  // move assignment
    EXPECT_EQ(a2.get_id(),       1);
    EXPECT_EQ(a2.get_name(),     "Omar");
    EXPECT_EQ(a2.get_email(),    "omar@test.com");
    EXPECT_EQ(a2.get_password(), "1234");
    EXPECT_EQ(a2.get_role(),     "admin");
}

/* ================= ROLE IS ALWAYS ADMIN ================= */

TEST(AdminTest, RoleIsAlwaysAdmin)
{
    Admin a1(1, "A", "a@test.com", "p1");
    Admin a2(2, "B", "b@test.com", "p2");
    Admin a3(3, "C", "c@test.com", "p3");
    EXPECT_EQ(a1.get_role(), "admin");
    EXPECT_EQ(a2.get_role(), "admin");
    EXPECT_EQ(a3.get_role(), "admin");
}

/* ================= AUTHENTICATE - ALL BRANCHES ================= */

TEST(AdminTest, Auth_Success)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
}

TEST(AdminTest, Auth_WrongPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
}

TEST(AdminTest, Auth_WrongEmail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
}

TEST(AdminTest, Auth_BothWrong)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("x", "y"));
}

TEST(AdminTest, Auth_CorrectEmailEmptyPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("omar@test.com", ""));
}

TEST(AdminTest, Auth_EmptyEmailCorrectPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    EXPECT_FALSE(a.Authenticate("", "1234"));
}

TEST(AdminTest, Auth_EmptyCredentialsMatch)
{
    Admin a(1, "", "", "");
    EXPECT_TRUE(a.Authenticate("", ""));
}

TEST(AdminTest, Auth_EmptyAdmin_WrongEmail)
{
    Admin a(1, "", "", "");
    EXPECT_FALSE(a.Authenticate("x@test.com", ""));
}

TEST(AdminTest, Auth_EmptyAdmin_WrongPassword)
{
    Admin a(1, "", "", "");
    EXPECT_FALSE(a.Authenticate("", "wrongpass"));
}

/* ================= SETTERS ================= */

TEST(AdminTest, Setters)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_id(55);
    a.set_name("New");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_EQ(a.get_id(),       55);
    EXPECT_EQ(a.get_name(),     "New");
    EXPECT_EQ(a.get_email(),    "new@test.com");
    EXPECT_EQ(a.get_password(), "newpass");
    EXPECT_EQ(a.get_role(),     "admin");
}

TEST(AdminTest, Auth_AfterSetters_Success)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_TRUE(a.Authenticate("new@test.com", "newpass"));
}

TEST(AdminTest, Auth_AfterSetters_WrongPassword)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("new@test.com", "wrong"));
}

TEST(AdminTest, Auth_AfterSetters_WrongEmail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("other@test.com", "newpass"));
}

TEST(AdminTest, Auth_AfterSetters_OldCredsFail)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_email("new@test.com");
    a.set_password("newpass");
    EXPECT_FALSE(a.Authenticate("omar@test.com", "1234"));
}

/* ================= ROLE MUTATION ================= */

TEST(AdminTest, RoleCanBeChanged)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    a.set_role("doctor");
    EXPECT_EQ(a.get_role(), "doctor");
    EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
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

TEST(AdminTest, MultipleInstances)
{
    Admin a1(1, "A1", "a1@test.com", "pass1");
    Admin a2(2, "A2", "a2@test.com", "pass2");
    Admin a3(3, "A3", "a3@test.com", "pass3");

    EXPECT_TRUE(a1.Authenticate("a1@test.com", "pass1"));
    EXPECT_TRUE(a2.Authenticate("a2@test.com", "pass2"));
    EXPECT_TRUE(a3.Authenticate("a3@test.com", "pass3"));

    EXPECT_FALSE(a1.Authenticate("a2@test.com", "pass2"));
    EXPECT_FALSE(a2.Authenticate("a1@test.com", "pass1"));
}

/* ================= STRESS ================= */

TEST(AdminTest, AuthStress)
{
    Admin a(1, "Omar", "omar@test.com", "1234");
    for (int i = 0; i < 50; i++)
    {
        EXPECT_TRUE(a.Authenticate("omar@test.com", "1234"));
        EXPECT_FALSE(a.Authenticate("omar@test.com", "wrong"));
        EXPECT_FALSE(a.Authenticate("wrong@test.com", "1234"));
    }
}
