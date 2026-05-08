#include <gtest/gtest.h>
#include "Appointment.h"

/* ================= DEFAULT CONSTRUCTOR ================= */

TEST(AppointmentTest, DefaultConstructor)
{
    Appointment a;
    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_PatientId(),     0);
    EXPECT_EQ(a.get_DoctorId(),      0);
    EXPECT_EQ(a.get_Date(),          "");
    EXPECT_EQ(a.get_Time(),          "");
    EXPECT_EQ(a.get_Status(),        Status::Scheduled);
}

/* ================= PARAMETERIZED CONSTRUCTOR ================= */

TEST(AppointmentTest, Constructor_Scheduled)
{
    AppointmentInfo info{"PatientA", "DoctorA", "2026-01-01", "08:00"};
    Appointment a(1, 10, 100, info, Status::Scheduled);
    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_PatientId(),     10);
    EXPECT_EQ(a.get_DoctorId(),      100);
    EXPECT_EQ(a.get_Date(),          "2026-01-01");
    EXPECT_EQ(a.get_Time(),          "08:00");
    EXPECT_EQ(a.get_Status(),        Status::Scheduled);
}

TEST(AppointmentTest, Constructor_Cancelled)
{
    AppointmentInfo info{"PatientB", "DoctorB", "2026-02-01", "09:00"};
    Appointment a(2, 20, 200, info, Status::Cancelled);
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, Constructor_Completed)
{
    AppointmentInfo info{"PatientC", "DoctorC", "2026-03-01", "10:00"};
    Appointment a(3, 30, 300, info, Status::Completed);
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= CANCEL ================= */

// condition TRUE → changes to Cancelled
TEST(AppointmentTest, Cancel_FromScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// condition FALSE (Cancelled) → no change
TEST(AppointmentTest, Cancel_FromCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// condition FALSE (Completed) → no change
TEST(AppointmentTest, Cancel_FromCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= COMPLETE ================= */

// condition TRUE → changes to Completed
TEST(AppointmentTest, Complete_FromScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// condition FALSE (Completed) → no change
TEST(AppointmentTest, Complete_FromCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// condition FALSE (Cancelled) → no change
TEST(AppointmentTest, Complete_FromCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ================= PRINT_ROW - ALL BRANCHES ================= */

// Scheduled branch
TEST(AppointmentTest, PrintRow_Scheduled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    EXPECT_NO_THROW(a.print_row());
}

// Cancelled branch (Scheduled=FALSE, Cancelled=TRUE)
TEST(AppointmentTest, PrintRow_Cancelled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// Completed branch (Scheduled=FALSE, Cancelled=FALSE → else)
TEST(AppointmentTest, PrintRow_Completed)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, PrintRow_AfterCancelTransition)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, PrintRow_AfterCompleteTransition)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}

/* ================= MIXED CHAINS ================= */

TEST(AppointmentTest, CancelThenComplete_StaysCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, CompleteThenCancel_StaysCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, DoubleCancel)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, DoubleComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= FALSE BRANCH + PRINT_ROW COMBOS ================= */

TEST(AppointmentTest, Cancel_Cancelled_ThenPrint)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, Cancel_Completed_ThenPrint)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, Complete_Cancelled_ThenPrint)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, Complete_Completed_ThenPrint)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_NO_THROW(a.print_row());
}

/* ================= GETTERS - ALL 3 STATUSES ================= */

TEST(AppointmentTest, Getters_Scheduled)
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

TEST(AppointmentTest, Getters_Cancelled)
{
    AppointmentInfo info{"Carol", "Dave", "2026-06-15", "2PM"};
    Appointment a(5, 10, 15, info, Status::Cancelled);
    EXPECT_EQ(a.get_AppointmentId(), 5);
    EXPECT_EQ(a.get_PatientId(),     10);
    EXPECT_EQ(a.get_DoctorId(),      15);
    EXPECT_EQ(a.get_Status(),        Status::Cancelled);
}

TEST(AppointmentTest, Getters_Completed)
{
    AppointmentInfo info{"Eve", "Frank", "2026-12-31", "11PM"};
    Appointment a(99, 50, 75, info, Status::Completed);
    EXPECT_EQ(a.get_AppointmentId(), 99);
    EXPECT_EQ(a.get_PatientId(),     50);
    EXPECT_EQ(a.get_DoctorId(),      75);
    EXPECT_EQ(a.get_Status(),        Status::Completed);
}
