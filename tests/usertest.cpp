#include <gtest/gtest.h>
#include "user.h"

TEST(UserTest, EmptyUserAuthFails) {
    User u;
    EXPECT_FALSE(u.Authenticate("", ""));
}

TEST(UserTest, WrongEmail) {
    User u(1,"Omar","omar@test.com","1234","patient");
    EXPECT_FALSE(u.Authenticate("wrong@test.com","1234"));
}

TEST(UserTest, WrongPassword) {
    User u(1,"Omar","omar@test.com","1234","patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com","wrong"));
}

TEST(UserTest, CorrectAuth) {
    User u(1,"Omar","omar@test.com","1234","patient");
    EXPECT_TRUE(u.Authenticate("omar@test.com","1234"));
}
