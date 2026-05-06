#include <gtest/gtest.h>
#include "Patient.h"

TEST(PatientTest, Constructor)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");

    EXPECT_EQ(p.get_role(), "patient");
    EXPECT_EQ(p.get_phone(), "010");
}

TEST(PatientTest, PhoneSetter)
{
    Patient p(1, "Ali", "a@test.com", "1234", "010");

    p.set_phone("011");

    EXPECT_EQ(p.get_phone(), "011");
}