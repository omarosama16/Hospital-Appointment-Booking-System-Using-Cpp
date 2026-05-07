#include <gtest/gtest.h>
#include "Appointment.h"

/* ===================== DEFAULT ===================== */

TEST(AppointmentTest, DefaultConstructorFullState)
{
    Appointment a;

    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_PatientId(), 0);
    EXPECT_EQ(a.get_DoctorId(), 0);
    EXPECT_EQ(a.get_Date(), "");
    EXPECT_EQ(a.get_Time(), "");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

/* ===================== PARAMETERIZED ===================== */

TEST(AppointmentTest, ParameterizedConstructor)
{
    Appointment a(1, 2, 3, "p", "d", "2026", "10AM", Status::Scheduled);

    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_PatientId(), 2);
    EXPECT_EQ(a.get_DoctorId(), 3);
    EXPECT_EQ(a.get_Date(), "2026");
    EXPECT_EQ(a.get_Time(), "10AM");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

/* ===================== CANCEL / COMPLETE ===================== */

TEST(AppointmentTest, CancelAndComplete)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);

    a.complete(); // should not override cancelled
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteScheduledAppointment)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ===================== EDGE CASES ===================== */

TEST(AppointmentTest, CancelTwiceStaysCancelled)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.cancel();
    a.cancel();

    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteTwiceStaysCompleted)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.complete();
    a.complete();

    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CancelPreventsCompletionFully)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.cancel();
    a.complete();

    EXPECT_NE(a.get_Status(), Status::Completed);
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, ConstructorWithCancelledState)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Cancelled);

    EXPECT_EQ(a.get_Status(), Status::Cancelled);

    a.complete(); // should not change
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ===================== PRINT SAFETY ===================== */

TEST(AppointmentTest, PrintRowDoesNotCrash)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, PrintRowAfterCancel)
{
    Appointment a(1, 1, 1, "p", "d", "2026", "10AM", Status::Scheduled);

    a.cancel();

    EXPECT_NO_THROW(a.print_row());
}