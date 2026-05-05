#include <gtest/gtest.h>
#include "patient.h"

TEST(PatientTest, ConstructorAndUserInheritance) {
    patient p(1, "Omar", "omar@test.com", "1234", "01012345678");

    EXPECT_EQ(p.get_id(), 1);
    EXPECT_EQ(p.get_name(), "Omar");
    EXPECT_EQ(p.get_email(), "omar@test.com");
    EXPECT_EQ(p.get_password(), "1234");

    // IMPORTANT: your constructor sets role = "Patient"
    EXPECT_EQ(p.get_role(), "Patient");
}

TEST(PatientTest, PhoneGetterWorks) {
    patient p(2, "Ali", "ali@test.com", "pass", "0110000000");

    EXPECT_EQ(p.get_phone(), "0110000000");
}

TEST(PatientTest, PhoneSetterWorks) {
    patient p(3, "Sara", "sara@test.com", "pass", "000");

    p.set_phone("0123456789");

    EXPECT_EQ(p.get_phone(), "0123456789");
}

TEST(PatientTest, MultipleUpdatesOnPhone) {
    patient p(4, "Mona", "mona@test.com", "pass", "111");

    p.set_phone("222");
    EXPECT_EQ(p.get_phone(), "222");

    p.set_phone("333");
    EXPECT_EQ(p.get_phone(), "333");
}

TEST(PatientTest, ObjectDoesNotCrashOnAccess) {
    patient p(5, "Test", "t@test.com", "pass", "999");

    EXPECT_NO_THROW({
        p.get_phone();
        p.get_id();
        p.get_name();
        p.get_email();
        p.get_role();
    });
}
