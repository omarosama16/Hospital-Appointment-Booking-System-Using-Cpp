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

// FIX: covers parameterized constructor with Cancelled status directly
TEST(AppointmentTest, ParameterizedConstructorCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Cancelled);
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// FIX: covers parameterized constructor with Completed status directly
TEST(AppointmentTest, ParameterizedConstructorCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Completed);
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ===================== CANCEL ===================== */

// covers if(status == Scheduled) true branch in cancel()
TEST(AppointmentTest, CancelScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// covers false branch of if(status == Scheduled) in cancel() — status is Cancelled
TEST(AppointmentTest, CancelWhenAlreadyCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// covers false branch of if(status == Scheduled) in cancel() — status is Completed
TEST(AppointmentTest, CancelWhenAlreadyCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// FIX: directly constructed Completed — cancel() false branch without going through complete()
TEST(AppointmentTest, CancelDirectlyCompletedStatus)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CancelTwiceStaysCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ===================== COMPLETE ===================== */

// covers if(status == Scheduled) true branch in complete()
TEST(AppointmentTest, CompleteScheduledAppointment)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// covers false branch of if(status == Scheduled) in complete() — status is Completed
TEST(AppointmentTest, CompleteWhenAlreadyCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// covers false branch of if(status == Scheduled) in complete() — status is Cancelled
TEST(AppointmentTest, CompleteWhenAlreadyCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// FIX: directly constructed Completed — complete() false branch without transition
TEST(AppointmentTest, CompleteDirectlyCompletedStatus)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CompleteTwiceStaysCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ===================== CANCEL + COMPLETE INTERACTION ===================== */

TEST(AppointmentTest, CancelAndComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
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
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ===================== PRINT SAFETY ===================== */

// covers if(status == Scheduled) true branch in print_row()
TEST(AppointmentTest, PrintRowScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    EXPECT_NO_THROW(a.print_row());
}

// covers else if(status == Cancelled) true branch in print_row()
TEST(AppointmentTest, PrintRowCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// covers else branch in print_row() — status is Completed
// also covers false branch of else if(status == Cancelled)
TEST(AppointmentTest, PrintRowCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}

// FIX: directly constructed Completed — ensures else if false branch is hit independently
TEST(AppointmentTest, PrintRowDirectlyCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

/* ===================== GETTERS ===================== */

// FIX: explicitly exercises all getters to ensure condition coverage on return values
TEST(AppointmentTest, AllGetters)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(10, 20, 30, info, Status::Scheduled);
    EXPECT_EQ(a.get_AppointmentId(), 10);
    EXPECT_EQ(a.get_PatientId(),     20);
    EXPECT_EQ(a.get_DoctorId(),      30);
    EXPECT_EQ(a.get_Date(),          "2026-01-01");
    EXPECT_EQ(a.get_Time(),          "9AM");
    EXPECT_EQ(a.get_Status(),        Status::Scheduled);
}
