#include <gtest/gtest.h>
#include "Doctor.h"

TEST(DoctorTest, Constructor)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    EXPECT_EQ(d.get_role(), "doctor");
    EXPECT_EQ(d.getSpecialization(), "Cardiology");
}

TEST(DoctorTest, Availability)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    EXPECT_TRUE(d.getAvailability().empty());

    d.addAvailability("10AM");
    d.addAvailability("11AM");

    EXPECT_EQ(d.getAvailability().size(), 2);

    d.clearAvailability();

    EXPECT_TRUE(d.getAvailability().empty());
}