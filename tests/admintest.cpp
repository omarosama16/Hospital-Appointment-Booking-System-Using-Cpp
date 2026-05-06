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

TEST(AdminTest, ConstructorIsCoveredViaStackAndHeap) {
    // Stack allocation (covers normal constructor call)
    Admin a1(1, "StackUser", "stack@test.com", "123");

    EXPECT_EQ(a1.get_role(), "Admin");

    // Heap allocation (often needed for full coverage tools)
    Admin* a2 = new Admin(2, "HeapUser", "heap@test.com", "456");

    EXPECT_EQ(a2->get_id(), 2);
    EXPECT_EQ(a2->get_name(), "HeapUser");
    EXPECT_EQ(a2->get_role(), "Admin");

    delete a2; // ensures destructor path (from User) is also hit safely
}

TEST(AdminTest, PolymorphismCoversBasePointerConstruction) {
    User* user = new Admin(3, "PolyUser", "poly@test.com", "789");

    EXPECT_EQ(user->get_role(), "Admin");
    EXPECT_TRUE(user->Authenticate("poly@test.com", "789"));

    delete user;
}
