#include <gtest/gtest.h>
#include "system.h"

// helper: setup clean system state
static HospitalSystem setup(int &docId, int &patientId)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr Who", "doc@mail.com", "Neuro");
    system.registerNewPatient("Ali", "ali@mail.com", "010");

    // patient login required for booking
    system.login("ali@mail.com", "default");

    auto users = system.adminViewAllUsers();

    docId = -1;
    patientId = -1;

    for (auto u : users)
    {
        if (u->get_role() == "doctor") docId = u->get_id();
        if (u->get_role() == "patient") patientId = u->get_id();
    }

    return system;
}

TEST(SystemTest, Login_AllBranches)
{
    HospitalSystem system;

    EXPECT_FALSE(system.login("", ""));
    EXPECT_FALSE(system.login("wrong", "wrong"));
}

TEST(SystemTest, AdminUsersAndDoctorCreation)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "a@mail.com", "Cardio");
    system.registerNewPatient("P1", "p1@mail.com", "111");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 3); // admin + doctor + patient
}

TEST(SystemTest, BookAppointment_FullBranchCoverage)
{
    int docId, patientId;
    HospitalSystem system = setup(docId, patientId);

    ASSERT_NE(docId, -1);
    ASSERT_NE(patientId, -1);

    // success path
    EXPECT_TRUE(system.bookAppointment(docId, "2026-01-01", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);

    // conflict path
    EXPECT_FALSE(system.bookAppointment(docId, "2026-01-01", "10AM"));

    // wrong doctor id path
    EXPECT_FALSE(system.bookAppointment(9999, "2026", "10AM"));
}

TEST(SystemTest, CancelAppointment_AllPaths)
{
    int docId, patientId;
    HospitalSystem system = setup(docId, patientId);

    system.bookAppointment(docId, "2026", "10AM");
    auto apps = system.viewMyAppointments();

    ASSERT_EQ(apps.size(), 1);

    int apptId = apps[0].get_AppointmentId();

    // success cancel
    EXPECT_TRUE(system.cancelAppointmentPatient(apptId));

    // already cancelled → false path
    EXPECT_FALSE(system.cancelAppointmentPatient(apptId));

    // wrong id
    EXPECT_FALSE(system.cancelAppointmentPatient(999));
}

TEST(SystemTest, ViewMyAppointments_Branches)
{
    int docId, patientId;
    HospitalSystem system = setup(docId, patientId);

    // patient view
    system.bookAppointment(docId, "2026", "10AM");
    auto apps = system.viewMyAppointments();

    EXPECT_EQ(apps.size(), 1);

    // logout effect (simulate by new system user state)
    HospitalSystem system2;
    auto empty = system2.viewMyAppointments();
    EXPECT_EQ(empty.size(), 0);
}

TEST(SystemTest, DoctorScheduleAndComplete)
{
    int docId, patientId;
    HospitalSystem system = setup(docId, patientId);

    system.bookAppointment(docId, "2026", "10AM");

    // doctor login
    system.login("doc@mail.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    int apptId = schedule[0].get_AppointmentId();

    // complete success
    EXPECT_TRUE(system.completeAppointmentDoctor(apptId));

    // complete again should fail
    EXPECT_FALSE(system.completeAppointmentDoctor(apptId));

    // wrong id
    EXPECT_FALSE(system.completeAppointmentDoctor(999));
}

TEST(SystemTest, AdminViewsEverything)
{
    int docId, patientId;
    HospitalSystem system = setup(docId, patientId);

    system.bookAppointment(docId, "2026", "10AM");

    auto apps = system.adminViewAllAppointments();
    auto users = system.adminViewAllUsers();

    EXPECT_GE(apps.size(), 1);
    EXPECT_GE(users.size(), 3);
}
