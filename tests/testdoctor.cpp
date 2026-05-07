#include <gtest/gtest.h>
#include "Doctor.h"

TEST(DoctorTest, Constructor)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    EXPECT_EQ(d.get_role(), "doctor");
    EXPECT_EQ(d.getSpecialization(), "Cardiology");
}

TEST(DoctorTest, AvailabilityEmpty)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    EXPECT_TRUE(d.getAvailability().empty());
}

TEST(DoctorTest, AddAvailabilityMultiple)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    d.addAvailability("10AM");
    d.addAvailability("11AM");
    d.addAvailability("12PM");

    EXPECT_EQ(d.getAvailability().size(), 3);
}

TEST(DoctorTest, ClearAvailabilityEdge)
{
    Doctor d(1, "Omar", "o@test.com", "1234", "Cardiology");

    d.addAvailability("10AM");
    d.clearAvailability();

    EXPECT_TRUE(d.getAvailability().empty());

    // clearing empty again (edge branch)
    d.clearAvailability();
    EXPECT_TRUE(d.getAvailability().empty());
}