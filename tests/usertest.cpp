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

TEST(UserTest, ConstructorPatient)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_EQ(u.get_id(),       1);
    EXPECT_EQ(u.get_name(),     "Omar");
    EXPECT_EQ(u.get_email(),    "omar@test.com");
    EXPECT_EQ(u.get_password(), "1234");
    EXPECT_EQ(u.get_role(),     "patient");
}

TEST(UserTest, ConstructorDoctor)
{
    User u(2, "Sara", "sara@test.com", "abcd", "doctor");
    EXPECT_EQ(u.get_id(),       2);
    EXPECT_EQ(u.get_name(),     "Sara");
    EXPECT_EQ(u.get_email(),    "sara@test.com");
    EXPECT_EQ(u.get_password(), "abcd");
    EXPECT_EQ(u.get_role(),     "doctor");
}

TEST(UserTest, ConstructorAdmin)
{
    User u(3, "Admin", "admin@test.com", "root", "admin");
    EXPECT_EQ(u.get_id(),       3);
    EXPECT_EQ(u.get_name(),     "Admin");
    EXPECT_EQ(u.get_email(),    "admin@test.com");
    EXPECT_EQ(u.get_password(), "root");
    EXPECT_EQ(u.get_role(),     "admin");
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

/* ================= AUTHENTICATE - ALL CONDITION BRANCHES ================= */

// email=TRUE  && password=TRUE  → true
TEST(UserTest, Authenticate_CorrectEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
}

// email=TRUE  && password=FALSE → false
TEST(UserTest, Authenticate_CorrectEmailWrongPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
}

// email=FALSE (short-circuit, password never evaluated) → false
TEST(UserTest, Authenticate_WrongEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
}

// email=FALSE && password=FALSE → false
TEST(UserTest, Authenticate_BothWrong)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("wrong@test.com", "wrong"));
}

// email=TRUE  && password=FALSE: email matches but password is empty
TEST(UserTest, Authenticate_CorrectEmailEmptyPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", ""));
}

// email=FALSE: empty email with correct password
TEST(UserTest, Authenticate_EmptyEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("", "1234"));
}

// email=TRUE  && password=TRUE: default user authenticates with empty strings
TEST(UserTest, Authenticate_DefaultUserEmptyCredentials)
{
    User u;
    EXPECT_TRUE(u.Authenticate("", ""));
}

// email=FALSE: non-empty email against default empty email
TEST(UserTest, Authenticate_DefaultUser_WrongEmail)
{
    User u;
    EXPECT_FALSE(u.Authenticate("someone@test.com", ""));
}

// email=TRUE && password=FALSE: empty email correct, password wrong
TEST(UserTest, Authenticate_DefaultUser_WrongPassword)
{
    User u;
    EXPECT_FALSE(u.Authenticate("", "wrongpass"));
}

/* ================= AUTHENTICATE AFTER SETTERS ================= */

// Verify Authenticate works correctly after using setters
TEST(UserTest, Authenticate_AfterSetEmail_Success)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_TRUE(u.Authenticate("new@test.com", "newpass"));
}

TEST(UserTest, Authenticate_AfterSetEmail_WrongPassword)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_FALSE(u.Authenticate("new@test.com", "wrong"));
}

TEST(UserTest, Authenticate_AfterSetEmail_WrongEmail)
{
    User u;
    u.set_email("new@test.com");
    u.set_password("newpass");
    EXPECT_FALSE(u.Authenticate("other@test.com", "newpass"));
}

/* ================= GETTERS CROSS-CHECK ================= */

// Ensure getters return updated values after setters overwrite constructor values
TEST(UserTest, GettersAfterOverwrite)
{
    User u(1, "Original", "orig@test.com", "origpass", "patient");

    u.set_name("Updated");
    u.set_email("updated@test.com");
    u.set_password("newpass");
    u.set_role("doctor");
    u.set_id(42);

    EXPECT_EQ(u.get_id(),       42);
    EXPECT_EQ(u.get_name(),     "Updated");
    EXPECT_EQ(u.get_email(),    "updated@test.com");
    EXPECT_EQ(u.get_password(), "newpass");
    EXPECT_EQ(u.get_role(),     "doctor");

    // Authenticate with new credentials
    EXPECT_TRUE(u.Authenticate("updated@test.com", "newpass"));
    EXPECT_FALSE(u.Authenticate("orig@test.com", "origpass"));
}
