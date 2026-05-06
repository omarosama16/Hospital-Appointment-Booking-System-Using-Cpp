#include <gtest/gtest.h>
#include "Appointment.h"

TEST(AppointmentTest, DefaultConstructor)
{
    Appointment a;

    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_Status(), "");
}

TEST(AppointmentTest, ParameterizedConstructor)
{
    Appointment a(1, 2, 3, "p", "d", "2026", "10AM", "Scheduled");

    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_PatientId(), 2);
    EXPECT_EQ(a.get_DoctorId(), 3);
    EXPECT_EQ(a.get_Status(), "Scheduled");
}

TEST(AppointmentTest, CancelAndComplete)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");

    a.complete();
    EXPECT_EQ(a.get_Status(), "Cancelled");
}

TEST(AppointmentTest, PrintRowDoesNotCrash)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", "Scheduled");

    EXPECT_NO_THROW(a.print_row());
}