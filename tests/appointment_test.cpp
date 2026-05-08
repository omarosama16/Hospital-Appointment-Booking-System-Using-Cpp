#include <gtest/gtest.h>
#include "Appointment.h"
#include <sstream>

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

/* ================= PARAMETERIZED CONSTRUCTOR - ALL 3 STATUSES ================= */

TEST(AppointmentTest, ConstructorScheduled)
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

TEST(AppointmentTest, ConstructorCancelled)
{
    AppointmentInfo info{"PatientB", "DoctorB", "2026-02-01", "09:00"};
    Appointment a(2, 20, 200, info, Status::Cancelled);

    EXPECT_EQ(a.get_AppointmentId(), 2);
    EXPECT_EQ(a.get_PatientId(),     20);
    EXPECT_EQ(a.get_DoctorId(),      200);
    EXPECT_EQ(a.get_Date(),          "2026-02-01");
    EXPECT_EQ(a.get_Time(),          "09:00");
    EXPECT_EQ(a.get_Status(),        Status::Cancelled);
}

TEST(AppointmentTest, ConstructorCompleted)
{
    AppointmentInfo info{"PatientC", "DoctorC", "2026-03-01", "10:00"};
    Appointment a(3, 30, 300, info, Status::Completed);

    EXPECT_EQ(a.get_AppointmentId(), 3);
    EXPECT_EQ(a.get_PatientId(),     30);
    EXPECT_EQ(a.get_DoctorId(),      300);
    EXPECT_EQ(a.get_Date(),          "2026-03-01");
    EXPECT_EQ(a.get_Time(),          "10:00");
    EXPECT_EQ(a.get_Status(),        Status::Completed);
}

/* ================= CANCEL() ================= */

// condition TRUE  → status changes
TEST(AppointmentTest, Cancel_FromScheduled_BecomeCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// condition FALSE (already Cancelled) → no change
TEST(AppointmentTest, Cancel_FromCancelled_NoChange)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// condition FALSE (Completed) → no change
TEST(AppointmentTest, Cancel_FromCompleted_NoChange)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= COMPLETE() ================= */

// condition TRUE → status changes
TEST(AppointmentTest, Complete_FromScheduled_BecomeCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// condition FALSE (already Completed) → no change
TEST(AppointmentTest, Complete_FromCompleted_NoChange)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

// condition FALSE (Cancelled) → no change
TEST(AppointmentTest, Complete_FromCancelled_NoChange)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ================= PRINT_ROW() - EVERY BRANCH EXPLICIT ================= */

// Branch 1: status == Scheduled → TRUE, else if never reached
TEST(AppointmentTest, PrintRow_Scheduled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    EXPECT_NO_THROW(a.print_row());
}

// Branch 2: status == Scheduled → FALSE, status == Cancelled → TRUE
TEST(AppointmentTest, PrintRow_Cancelled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// Branch 3: status == Scheduled → FALSE, status == Cancelled → FALSE → else
TEST(AppointmentTest, PrintRow_Completed)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

// Print after transition Scheduled → Cancelled (hits Cancelled branch via runtime change)
TEST(AppointmentTest, PrintRow_AfterCancelTransition)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// Print after transition Scheduled → Completed (hits else branch via runtime change)
TEST(AppointmentTest, PrintRow_AfterCompleteTransition)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

/* ================= MIXED TRANSITION CHAINS ================= */

TEST(AppointmentTest, CancelThenComplete_StaysCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.complete(); // should NOT change
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

TEST(AppointmentTest, CompleteThenCancel_StaysCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.cancel(); // should NOT change
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

/* ================= CONDITION PAIRS: cancel() FALSE branch both ways ================= */

TEST(AppointmentTest, Cancel_Cancelled_ThenPrintRow)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel(); // condition FALSE
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    EXPECT_NO_THROW(a.print_row()); // Cancelled branch in print_row
}

TEST(AppointmentTest, Cancel_Completed_ThenPrintRow)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel(); // condition FALSE
    EXPECT_EQ(a.get_Status(), Status::Completed);
    EXPECT_NO_THROW(a.print_row()); // else branch in print_row
}

/* ================= CONDITION PAIRS: complete() FALSE branch both ways ================= */

TEST(AppointmentTest, Complete_Cancelled_ThenPrintRow)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete(); // condition FALSE
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    EXPECT_NO_THROW(a.print_row()); // Cancelled branch in print_row
}

TEST(AppointmentTest, Complete_Completed_ThenPrintRow)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete(); // condition FALSE
    EXPECT_EQ(a.get_Status(), Status::Completed);
    EXPECT_NO_THROW(a.print_row()); // else branch in print_row
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
    EXPECT_EQ(a.get_Date(),          "2026-06-15");
    EXPECT_EQ(a.get_Time(),          "2PM");
    EXPECT_EQ(a.get_Status(),        Status::Cancelled);
}

TEST(AppointmentTest, Getters_Completed)
{
    AppointmentInfo info{"Eve", "Frank", "2026-12-31", "11PM"};
    Appointment a(99, 50, 75, info, Status::Completed);
    EXPECT_EQ(a.get_AppointmentId(), 99);
    EXPECT_EQ(a.get_PatientId(),     50);
    EXPECT_EQ(a.get_DoctorId(),      75);
    EXPECT_EQ(a.get_Date(),          "2026-12-31");
    EXPECT_EQ(a.get_Time(),          "11PM");
    EXPECT_EQ(a.get_Status(),        Status::Completed);
}
