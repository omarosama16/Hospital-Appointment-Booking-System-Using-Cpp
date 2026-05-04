#include <gtest/gtest.h>
#include "Doctor.h"

TEST(DoctorTest, NameCheck) {
    Doctor d("Omar");
    EXPECT_EQ(d.name, "Omar");
}
