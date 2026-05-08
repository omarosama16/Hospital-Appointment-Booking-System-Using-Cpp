#include <gtest/gtest.h>
#include "Appointment.h"

/* ================= DEFAULT ================= */

TEST(AppointmentTest, DefaultConstructor)
{
    Appointment a;

    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_PatientId(), 0);
    EXPECT_EQ(a.get_DoctorId(), 0);
    EXPECT_EQ(a.get_Date(), "");
    EXPECT_EQ(a.get_Time(), "");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

/* ================= CONSTRUCTOR STATES ================= */

TEST(AppointmentTest, ConstructorAllStates)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};

    Appointment a1(1, 1, 1, info, Status::Scheduled);
    EXPECT_EQ(a1.get_Status(), Status::Scheduled);

    Appointment a2(2, 2, 2, info, Status::Cancelled);
    EXPECT_EQ(a2.get_Status(), Status::Cancelled);

    Appointment a3(3, 3, 3, info, Status::Completed);
    EXPECT_EQ(a3.get_Status(), Status::Completed);
}

/* ================= CANCEL ================= */

TEST(AppointmentTest, CancelFromScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CancelFromCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CancelFromCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= COMPLETE ================= */

TEST(AppointmentTest, CompleteFromScheduled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CompleteFromCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentTest, CompleteFromCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

/* ================= MIXED CHAINS ================= */

TEST(AppointmentTest, CancelThenComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteThenCancel)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
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

/* ================= PRINT ROW - ALL CONDITION BRANCHES ================= */

// Covers: status == Scheduled → TRUE path
TEST(AppointmentTest, PrintRowScheduled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    EXPECT_NO_THROW(a.print_row());
}

// Covers: status == Scheduled → FALSE, status == Cancelled → TRUE path
TEST(AppointmentTest, PrintRowCancelled)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(2, 2, 2, info, Status::Cancelled);
    EXPECT_NO_THROW(a.print_row());
}

// Covers: status == Scheduled → FALSE, status == Cancelled → FALSE → else path
TEST(AppointmentTest, PrintRowCompleted)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(3, 3, 3, info, Status::Completed);
    EXPECT_NO_THROW(a.print_row());
}

// Covers print_row after state transition: Scheduled → Cancelled
TEST(AppointmentTest, PrintRowAfterCancel)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.cancel();
    EXPECT_NO_THROW(a.print_row()); // now Cancelled
}

// Covers print_row after state transition: Scheduled → Completed
TEST(AppointmentTest, PrintRowAfterComplete)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(1, 1, 1, info, Status::Scheduled);
    a.complete();
    EXPECT_NO_THROW(a.print_row()); // now Completed
}

/* ================= GETTERS FULL COVERAGE ================= */

TEST(AppointmentTest, GettersCoverage)
{
    AppointmentInfo info{"Alice", "Bob", "2026-01-01", "9AM"};
    Appointment a(10, 20, 30, info, Status::Scheduled);

    EXPECT_EQ(a.get_AppointmentId(), 10);
    EXPECT_EQ(a.get_PatientId(), 20);
    EXPECT_EQ(a.get_DoctorId(), 30);
    EXPECT_EQ(a.get_Date(), "2026-01-01");
    EXPECT_EQ(a.get_Time(), "9AM");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

// Getters with Cancelled status
TEST(AppointmentTest, GettersCancelledStatus)
{
    AppointmentInfo info{"Carol", "Dave", "2026-06-15", "2PM"};
    Appointment a(5, 10, 15, info, Status::Cancelled);

    EXPECT_EQ(a.get_AppointmentId(), 5);
    EXPECT_EQ(a.get_PatientId(), 10);
    EXPECT_EQ(a.get_DoctorId(), 15);
    EXPECT_EQ(a.get_Date(), "2026-06-15");
    EXPECT_EQ(a.get_Time(), "2PM");
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

// Getters with Completed status
TEST(AppointmentTest, GettersCompletedStatus)
{
    AppointmentInfo info{"Eve", "Frank", "2026-12-31", "11PM"};
    Appointment a(99, 50, 75, info, Status::Completed);

    EXPECT_EQ(a.get_AppointmentId(), 99);
    EXPECT_EQ(a.get_PatientId(), 50);
    EXPECT_EQ(a.get_DoctorId(), 75);
    EXPECT_EQ(a.get_Date(), "2026-12-31");
    EXPECT_EQ(a.get_Time(), "11PM");
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

/* ================= CANCEL/COMPLETE CONDITION PAIRS ================= */

// cancel(): status != Scheduled (Cancelled) → condition FALSE, no change
TEST(AppointmentTest, CancelConditionFalseViaCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.print_row(); // extra branch hit on Cancelled
}

// cancel(): status != Scheduled (Completed) → condition FALSE, no change
TEST(AppointmentTest, CancelConditionFalseViaCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
    a.print_row(); // extra branch hit on Completed
}

// complete(): status != Scheduled (Cancelled) → condition FALSE, no change
TEST(AppointmentTest, CompleteConditionFalseViaCancelled)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
    a.print_row(); // Cancelled branch in print_row
}

// complete(): status != Scheduled (Completed) → condition FALSE, no change
TEST(AppointmentTest, CompleteConditionFalseViaCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};
    Appointment a(1, 1, 1, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
    a.print_row(); // Completed branch in print_row
}
