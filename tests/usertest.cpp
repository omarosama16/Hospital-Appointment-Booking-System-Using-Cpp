#include <gtest/gtest.h>
#include "user.h"

TEST(UserTest, DefaultConstructor) {
    User u;

    EXPECT_EQ(u.get_id(), 0);
    EXPECT_EQ(u.get_name(), "");
    EXPECT_EQ(u.get_email(), "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(), "");
}

TEST(UserTest, ConstructorAndGetters) {
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_EQ(u.get_id(), 1);
    EXPECT_EQ(u.get_name(), "Omar");
    EXPECT_EQ(u.get_email(), "omar@test.com");
    EXPECT_EQ(u.get_password(), "1234");
    EXPECT_EQ(u.get_role(), "patient");
}

TEST(UserTest, SettersWorkCorrectly) {
    User u;

    u.set_id(2);
    u.set_name("Ali");
    u.set_email("ali@test.com");
    u.set_password("pass");
    u.set_role("admin");

    EXPECT_EQ(u.get_id(), 2);
    EXPECT_EQ(u.get_name(), "Ali");
    EXPECT_EQ(u.get_email(), "ali@test.com");
    EXPECT_EQ(u.get_password(), "pass");
    EXPECT_EQ(u.get_role(), "admin");
}

TEST(UserTest, EmptyUserAuthFails) {
    User u;

    EXPECT_FALSE(u.Authenticate("", ""));
    EXPECT_FALSE(u.Authenticate("omar@test.com", ""));
    EXPECT_FALSE(u.Authenticate("", "1234"));
}

TEST(UserTest, WrongEmail) {
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
}

TEST(UserTest, WrongPassword) {
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
}

TEST(UserTest, CorrectAuth) {
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
}

TEST(UserTest, ChangeCredentialsAndAuthenticate) {
    User u(1, "Omar", "omar@test.com", "1234", "patient");

    u.set_email("new@test.com");
    u.set_password("newpass");

    EXPECT_TRUE(u.Authenticate("new@test.com", "newpass"));
    EXPECT_FALSE(u.Authenticate("omar@test.com", "1234"));
}
