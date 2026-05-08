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
    a.cancel(); // no change

    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CancelFromCompleted)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};

    Appointment a(1, 1, 1, info, Status::Completed);
    a.cancel(); // no change

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

/* ================= MIXED CHAINS (IMPORTANT FOR COVERAGE) ================= */

TEST(AppointmentTest, CancelThenComplete)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};

    Appointment a(1, 1, 1, info, Status::Scheduled);

    a.cancel();
    a.complete(); // should NOT change

    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentTest, CompleteThenCancel)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};

    Appointment a(1, 1, 1, info, Status::Scheduled);

    a.complete();
    a.cancel(); // should NOT change

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

/* ================= PRINT (ALL BRANCHES) ================= */

TEST(AppointmentTest, PrintAllStates)
{
    AppointmentInfo info{"p", "d", "2026", "10AM"};

    Appointment a1(1, 1, 1, info, Status::Scheduled);
    Appointment a2(2, 2, 2, info, Status::Cancelled);
    Appointment a3(3, 3, 3, info, Status::Completed);

    EXPECT_NO_THROW(a1.print_row());
    EXPECT_NO_THROW(a2.print_row());
    EXPECT_NO_THROW(a3.print_row());
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
