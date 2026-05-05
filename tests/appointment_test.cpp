#include <gtest/gtest.h>
#include "Appointment.h"

TEST(AppointmentTest, DefaultConstructor) {
    Appointment a;
    EXPECT_EQ(a.get_Status(), "");
}

TEST(AppointmentTest, CancelTwice) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");

    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");

    a.cancel(); 
    EXPECT_EQ(a.get_Status(), "Cancelled");
}

TEST(AppointmentTest, CompleteAfterCancel) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");

    a.cancel();
    a.complete();

    EXPECT_EQ(a.get_Status(), "Completed");
}

TEST(AppointmentTest, PrintRowDoesNotCrash) {
    Appointment a(1,1,1,"p","d","2026","10:00","scheduled");
    EXPECT_NO_THROW(a.print_row());
}
