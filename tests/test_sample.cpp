#include <gtest/gtest.h>
#include "Doctor.h"

TEST(DoctorTest, ConstructorAndGetters) {
    Doctor d(1, "Omar", "omar@test.com", "1234", "cardiology");

    EXPECT_EQ(d.getName(), "Omar");

    EXPECT_EQ(d.getId(), 1);
}
