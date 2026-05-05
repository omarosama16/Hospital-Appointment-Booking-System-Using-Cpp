#include <gtest/gtest.h>
#include "Doctor.h"
 
TEST(DoctorTest, ConstructorAndGetters) {
    Doctor d(1, "Omar", "omar@test.com", "1234", "Cardiology");
    EXPECT_EQ(d.get_id(), 1);
    EXPECT_EQ(d.get_name(), "Omar");
    EXPECT_EQ(d.get_email(), "omar@test.com");
    EXPECT_EQ(d.get_password(), "1234");
    EXPECT_EQ(d.getSpecialization(), "Cardiology");
}
 
TEST(DoctorTest, DefaultAvailabilityIsEmpty) {
    Doctor d(2, "Ahmed", "ahmed@test.com", "pass", "Neurology");
    EXPECT_TRUE(d.getAvailability().empty());
}
 
TEST(DoctorTest, AddAvailability) {
    Doctor d(3, "Sara", "sara@test.com", "pass", "Pediatrics");
    d.addAvailability("Monday 10:00");
    d.addAvailability("Tuesday 14:00");
    EXPECT_EQ(d.getAvailability().size(), 2);
    EXPECT_EQ(d.getAvailability()[0], "Monday 10:00");
}
 
TEST(DoctorTest, ClearAvailability) {
    Doctor d(4, "Mona", "mona@test.com", "pass", "Dermatology");
    d.addAvailability("Wednesday 09:00");
    d.clearAvailability();
    EXPECT_TRUE(d.getAvailability().empty());
}
 
TEST(DoctorTest, GetRole) {
    Doctor d(5, "Ali", "ali@test.com", "pass", "Surgery");
    EXPECT_EQ(d.get_role(), "doctor");
}
