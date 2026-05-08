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
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Scheduled);
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
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.complete(); // should not override cancelled
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteScheduledAppointment)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ===================== EDGE CASES ===================== */
TEST(AppointmentTest, CancelTwiceStaysCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteTwiceStaysCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CancelPreventsCompletionFully)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.complete();
    EXPECT_NE(a.get_Status(), Status::Completed);
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, ConstructorWithCancelledState)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.complete(); // should not change
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ===================== PRINT SAFETY ===================== */
TEST(AppointmentTest, PrintRowDoesNotCrash)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, PrintRowAfterCancel)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// FIX: added — covers the "Completed" branch in print_row() which was uncovered
TEST(AppointmentTest, PrintRowAfterComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}
