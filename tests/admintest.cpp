#include <gtest/gtest.h>
#include "admin.h"

TEST(AdminTest, ConstructorAndUserInheritance) {
    Admin a(1, "Omar", "omar@test.com", "1234");

    EXPECT_EQ(a.get_id(), 1);
    EXPECT_EQ(a.get_name(), "Omar");
    EXPECT_EQ(a.get_email(), "omar@test.com");
    EXPECT_EQ(a.get_password(), "1234");

    // IMPORTANT: matches your implementation ("Admin")
    EXPECT_EQ(a.get_role(), "Admin");
}

TEST(AdminTest, ObjectDoesNotCrashOnAccess) {
    Admin a(2, "Ali", "ali@test.com", "pass");

    EXPECT_NO_THROW({
        a.get_id();
        a.get_name();
        a.get_email();
        a.get_password();
        a.get_role();
    });
}

TEST(AdminTest, MultipleAdminsIndependentObjects) {
    Admin a1(1, "A1", "a1@test.com", "p1");
    Admin a2(2, "A2", "a2@test.com", "p2");

    EXPECT_NE(a1.get_id(), a2.get_id());
    EXPECT_NE(a1.get_email(), a2.get_email());
    EXPECT_EQ(a1.get_role(), a2.get_role());
}

TEST(AdminTest, BaseUserBehaviorConsistency) {
    Admin a(10, "TestAdmin", "admin@test.com", "secure");

    // Ensure it behaves like a normal User object
    EXPECT_TRUE(a.Authenticate("admin@test.com", "secure"));
    EXPECT_FALSE(a.Authenticate("wrong@test.com", "secure"));
    EXPECT_FALSE(a.Authenticate("admin@test.com", "wrong"));
}
