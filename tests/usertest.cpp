#include <gtest/gtest.h>
#include "user.h"

/* ================= DEFAULT CONSTRUCTOR ================= */

TEST(UserTest, DefaultConstructor)
{
    User u;
    EXPECT_EQ(u.get_id(),       0);
    EXPECT_EQ(u.get_name(),     "");
    EXPECT_EQ(u.get_email(),    "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(),     "");
}

/* ================= PARAMETERIZED CONSTRUCTOR ================= */

TEST(UserTest, Constructor_Patient)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_EQ(u.get_id(),       1);
    EXPECT_EQ(u.get_name(),     "Omar");
    EXPECT_EQ(u.get_email(),    "omar@test.com");
    EXPECT_EQ(u.get_password(), "1234");
    EXPECT_EQ(u.get_role(),     "patient");
}

TEST(UserTest, Constructor_Doctor)
{
    User u(2, "Sara", "sara@test.com", "abcd", "doctor");
    EXPECT_EQ(u.get_id(),       2);
    EXPECT_EQ(u.get_name(),     "Sara");
    EXPECT_EQ(u.get_email(),    "sara@test.com");
    EXPECT_EQ(u.get_password(), "abcd");
    EXPECT_EQ(u.get_role(),     "doctor");
}

TEST(UserTest, Constructor_Admin)
{
    User u(3, "Admin", "admin@test.com", "root", "admin");
    EXPECT_EQ(u.get_id(),       3);
    EXPECT_EQ(u.get_name(),     "Admin");
    EXPECT_EQ(u.get_email(),    "admin@test.com");
    EXPECT_EQ(u.get_password(), "root");
    EXPECT_EQ(u.get_role(),     "admin");
}

TEST(UserTest, Constructor_ZeroId)
{
    User u(0, "Zero", "z@test.com", "zpass", "patient");
    EXPECT_EQ(u.get_id(),   0);
    EXPECT_EQ(u.get_role(), "patient");
}

TEST(UserTest, Constructor_NegativeId)
{
    User u(-1, "Neg", "n@test.com", "npass", "doctor");
    EXPECT_EQ(u.get_id(),   -1);
    EXPECT_EQ(u.get_role(), "doctor");
}

TEST(UserTest, Constructor_EmptyStrings)
{
    User u(1, "", "", "", "");
    EXPECT_EQ(u.get_name(),     "");
    EXPECT_EQ(u.get_email(),    "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(),     "");
}

TEST(UserTest, Constructor_LongStrings)
{
    std::string longName(200, 'A');
    std::string longEmail(200, 'B');
    std::string longPass(200, 'C');
    std::string longRole(200, 'D');

    User u(99, longName, longEmail, longPass, longRole);
    EXPECT_EQ(u.get_name(),     longName);
    EXPECT_EQ(u.get_email(),    longEmail);
    EXPECT_EQ(u.get_password(), longPass);
    EXPECT_EQ(u.get_role(),     longRole);
}

/* ================= SETTERS ================= */

TEST(UserTest, SetId)
{
    User u;
    u.set_id(99);
    EXPECT_EQ(u.get_id(), 99);
}

TEST(UserTest, SetName)
{
    User u;
    u.set_name("Ali");
    EXPECT_EQ(u.get_name(), "Ali");
}

TEST(UserTest, SetEmail)
{
    User u;
    u.set_email("ali@test.com");
    EXPECT_EQ(u.get_email(), "ali@test.com");
}

TEST(UserTest, SetPassword)
{
    User u;
    u.set_password("pass");
    EXPECT_EQ(u.get_password(), "pass");
}

TEST(UserTest, SetRole)
{
    User u;
    u.set_role("doctor");
    EXPECT_EQ(u.get_role(), "doctor");
}

TEST(UserTest, AllSettersTogether)
{
    User u;
    u.set_id(99);
    u.set_name("Ali");
    u.set_email("ali@test.com");
    u.set_password("pass");
    u.set_role("doctor");

    EXPECT_EQ(u.get_id(),       99);
    EXPECT_EQ(u.get_name(),     "Ali");
    EXPECT_EQ(u.get_email(),    "ali@test.com");
    EXPECT_EQ(u.get_password(), "pass");
    EXPECT_EQ(u.get_role(),     "doctor");
}

TEST(UserTest, SettersOverwriteConstructor)
{
    User u(1, "Original", "orig@test.com", "origpass", "patient");
    u.set_id(42);
    u.set_name("Updated");
    u.set_email("updated@test.com");
    u.set_password("newpass");
    u.set_role("doctor");

    EXPECT_EQ(u.get_id(),       42);
    EXPECT_EQ(u.get_name(),     "Updated");
    EXPECT_EQ(u.get_email(),    "updated@test.com");
    EXPECT_EQ(u.get_password(), "newpass");
    EXPECT_EQ(u.get_role(),     "doctor");
}

/* ================= AUTHENTICATE - ALL CONDITION BRANCHES ================= */

// email=TRUE && password=TRUE → true
TEST(UserTest, Auth_CorrectBoth)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
}

// email=TRUE && password=FALSE → false
TEST(UserTest, Auth_CorrectEmailWrongPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
}

// email=FALSE (short-circuit) → false
TEST(UserTest, Auth_WrongEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
}

// email=FALSE && password=FALSE → false
TEST(UserTest, Auth_BothWrong)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("x", "y"));
}

// email=TRUE && password=FALSE: correct email, empty password
TEST(UserTest, Auth_CorrectEmailEmptyPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", ""));
}

// email=FALSE: empty email, correct password
TEST(UserTest, Auth_EmptyEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("", "1234"));
}

// email=TRUE && password=TRUE: default user empty credentials
TEST(UserTest, Auth_DefaultUserEmptyBoth)
{
    User u;
    EXPECT_TRUE(u.Authenticate("", ""));
}

// email=FALSE on default user
TEST(UserTest, Auth_DefaultUser_WrongEmail)
{
    User u;
    EXPECT_FALSE(u.Authenticate("someone@test.com", ""));
}

// email=TRUE && password=FALSE on default user
TEST(UserTest, Auth_DefaultUser_WrongPassword)
{
    User u;
    EXPECT_FALSE(u.Authenticate("", "wrongpass"));
}

// After setters - all 4 branches
TEST(UserTest, Auth_AfterSetters_Success)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_TRUE(u.Authenticate("new@test.com", "newpass"));
}

TEST(UserTest, Auth_AfterSetters_WrongPassword)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_FALSE(u.Authenticate("new@test.com", "wrong"));
}

TEST(UserTest, Auth_AfterSetters_WrongEmail)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_FALSE(u.Authenticate("other@test.com", "newpass"));
}

TEST(UserTest, Auth_AfterSetters_OldCredsFail)
{
    User u(1, "Omar", "orig@test.com", "origpass", "patient");
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_FALSE(u.Authenticate("orig@test.com", "origpass"));
}

/* ================= STRESS ================= */

TEST(UserTest, AuthStress)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    for (int i = 0; i < 50; i++)
    {
        EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
        EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
        EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
    }
}
