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

/* ===================== CANCEL ===================== */

TEST(AppointmentTest, CancelScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// FIX: covers false branch of if(status == Scheduled) in cancel() — already Cancelled
TEST(AppointmentTest, CancelWhenAlreadyCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel(); // status != Scheduled — no-op branch
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// FIX: covers false branch of if(status == Scheduled) in cancel() — already Completed
TEST(AppointmentTest, CancelWhenAlreadyCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.cancel(); // status != Scheduled — no-op branch
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

TEST(AppointmentTest, CompleteScheduledAppointment)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// FIX: covers false branch of if(status == Scheduled) in complete() — already Completed
TEST(AppointmentTest, CompleteWhenAlreadyCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.complete(); // status != Scheduled — no-op branch
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// FIX: covers false branch of if(status == Scheduled) in complete() — already Cancelled
TEST(AppointmentTest, CompleteWhenAlreadyCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete(); // status != Scheduled — no-op branch
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

/* ===================== CANCEL + COMPLETE INTERACTION ===================== */

TEST(AppointmentTest, CancelAndComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.complete(); // should not override cancelled
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

// FIX: covers else if(status == Cancelled) true branch in print_row()
TEST(AppointmentTest, PrintRowCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// FIX: covers else branch (Completed) in print_row()
// also covers false branch of else if(status == Cancelled)
TEST(AppointmentTest, PrintRowCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}
