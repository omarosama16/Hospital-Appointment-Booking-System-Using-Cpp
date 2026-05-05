#include <gtest/gtest.h>
#include "system.h"

// -------------------------
// Helper setup
// -------------------------
static HospitalSystem setup(int &docId, int &patId)
{
    HospitalSystem system;

    system.registerNewDoctor("Doc", "doc@mail.com", "Cardio");
    system.registerNewPatient("Pat", "pat@mail.com", "123");

    // Based on constructor order:
    // Admin=1, Doctor=2, Patient=3
    docId = 2;
    patId = 3;

    return system;
}

// -------------------------
// LOGIN FULL COVERAGE
// -------------------------
TEST(SystemTest, LoginAllPaths)
{
    HospitalSystem system;

    system.registerNewPatient("P", "p@mail.com", "123");

    EXPECT_TRUE(system.login("p@mail.com", "default")); // success
    EXPECT_FALSE(system.login("wrong@mail.com", "default")); // fail path
    EXPECT_FALSE(system.login("", "")); // empty branch path
}

// -------------------------
// REGISTER USER COVERAGE
// -------------------------
TEST(SystemTest, RegisterUsersIncreaseVector)
{
    HospitalSystem system;

    system.registerNewPatient("A", "a@mail.com", "1");
    system.registerNewPatient("B", "b@mail.com", "2");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 3); // admin + 2 patients
}

// -------------------------
// BOOKING FULL BRANCH COVERAGE
// -------------------------
TEST(SystemTest, BookAppointment_AllBranches)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    // FAIL: no login
    EXPECT_FALSE(system.bookAppointment(docId, "2026", "10AM"));

    // LOGIN as patient
    system.login("pat@mail.com", "default");

    // SUCCESS booking
    EXPECT_TRUE(system.bookAppointment(docId, "2026", "10AM"));

    // FAIL conflict branch
    EXPECT_FALSE(system.bookAppointment(docId, "2026", "10AM"));

    // FAIL invalid doctor branch
    EXPECT_FALSE(system.bookAppointment(999, "2026", "10AM"));
}

// -------------------------
// checkConflict FULL COVERAGE
// -------------------------
TEST(SystemTest, Conflict_AllConditions)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    system.login("pat@mail.com", "default");

    system.bookAppointment(docId, "2026", "10AM");

    // Same doctor/date/time → conflict TRUE
    EXPECT_FALSE(system.bookAppointment(docId, "2026", "10AM"));

    // Different time → should pass
    EXPECT_TRUE(system.bookAppointment(docId, "2026", "11AM"));

    // Different date → should pass
    EXPECT_TRUE(system.bookAppointment(docId, "2027", "10AM"));
}

// -------------------------
// CANCEL FULL COVERAGE
// -------------------------
TEST(SystemTest, Cancel_AllBranches)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    system.login("pat@mail.com", "default");

    system.bookAppointment(docId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    int id = apps[0].get_AppointmentId();

    // SUCCESS cancel
    EXPECT_TRUE(system.cancelAppointmentPatient(id));

    // FAIL cancel again (status changed branch)
    EXPECT_FALSE(system.cancelAppointmentPatient(id));

    // FAIL wrong id
    EXPECT_FALSE(system.cancelAppointmentPatient(999));
}

// -------------------------
// VIEW MY APPOINTMENTS
// -------------------------
TEST(SystemTest, ViewMyAppointments_AllBranches)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    // not logged in → empty branch
    auto empty = system.viewMyAppointments();
    EXPECT_TRUE(empty.empty());

    system.login("pat@mail.com", "default");
    system.bookAppointment(docId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

// -------------------------
// DOCTOR VIEW + COMPLETE
// -------------------------
TEST(SystemTest, Doctor_FullCoverage)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    system.login("pat@mail.com", "default");
    system.bookAppointment(docId, "2026", "10AM");

    system.login("doc@mail.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    int id = schedule[0].get_AppointmentId();

    // SUCCESS complete
    EXPECT_TRUE(system.completeAppointmentDoctor(id));

    // FAIL: already completed (status branch)
    EXPECT_FALSE(system.completeAppointmentDoctor(id));

    // FAIL invalid id
    EXPECT_FALSE(system.completeAppointmentDoctor(999));
}

// -------------------------
// ADMIN COVERAGE
// -------------------------
TEST(SystemTest, Admin_AllViews)
{
    int docId, patId;
    HospitalSystem system = setup(docId, patId);

    auto users = system.adminViewAllUsers();
    EXPECT_GE(users.size(), 2);

    auto apps = system.adminViewAllAppointments();
    EXPECT_EQ(apps.size(), 0);
}
