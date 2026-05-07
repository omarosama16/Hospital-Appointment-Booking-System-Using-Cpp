#include <gtest/gtest.h>
#include "Appointment.h"

TEST(AppointmentTest, DefaultConstructor)
{
    Appointment a;

    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_Status(), "");
}

TEST(AppointmentTest, FullConstructor)
{
    Appointment a(1, 2, 3, "p", "d", "2026", "10AM", "Scheduled");

    EXPECT_EQ(a.get_PatientId(), 2);
    EXPECT_EQ(a.get_DoctorId(), 3);
    EXPECT_EQ(a.get_Status(), "Scheduled");
}

TEST(AppointmentTest, CancelState)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");

    // cancel again (edge)
    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");
}

TEST(AppointmentTest, CompleteNormalFlow)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    a.complete();
    EXPECT_EQ(a.get_Status(), "Completed");
}

TEST(AppointmentTest, CompleteAfterCancelBlocked)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    a.cancel();
    a.complete(); // must stay cancelled
    EXPECT_EQ(a.get_Status(), "Cancelled");
}

TEST(AppointmentTest, PrintCoverage)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    EXPECT_NO_THROW(a.print_row());

    a.cancel();
    EXPECT_NO_THROW(a.print_row());
}