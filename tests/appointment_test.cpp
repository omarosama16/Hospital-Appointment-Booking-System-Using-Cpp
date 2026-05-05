#include <gtest/gtest.h>
#include "Appointment.h"

TEST(AppointmentTest, DefaultConstructor) {
    Appointment a;
    EXPECT_EQ(a.get_Status(), "");
}

TEST(AppointmentTest, CancelTwice) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");

    a.cancel();
    EXPECT_EQ(a.get_Status(), "cancelled");

    a.cancel(); // should stay cancelled
    EXPECT_EQ(a.get_Status(), "cancelled");
}

TEST(AppointmentTest, CompleteAfterCancel) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");

    a.cancel();
    a.complete();

    // depends on your logic — choose expected behavior
    EXPECT_EQ(a.get_Status(), "completed");
}

TEST(AppointmentTest, PrintRowDoesNotCrash) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");
    EXPECT_NO_THROW(a.print_row());
}
