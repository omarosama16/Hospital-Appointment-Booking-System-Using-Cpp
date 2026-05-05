#include <gtest/gtest.h>
#include "system.h"

static int getDoctorId(HospitalSystem &system)
{
    auto users = system.adminViewAllUsers();
    for (auto u : users)
        if (u->get_role() == "doctor")
            return u->get_id();
    return -1;
}

static void ensurePatientLogin(HospitalSystem &system)
{
    // ALWAYS re-login before actions (critical fix)
    ASSERT_TRUE(system.login("ali@mail.com", "default"));
}

TEST(HospitalSystemTest, Login_AllBranches)
{
    HospitalSystem system;

    EXPECT_FALSE(system.login("x@mail.com", "x"));
    EXPECT_TRUE(system.login("admin@mail.com", "admin123"));
}

TEST(HospitalSystemTest, AdminAndUserCreation)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("admin@mail.com", "admin123");

    auto users = system.adminViewAllUsers();
    EXPECT_GE(users.size(), 3);
}

TEST(HospitalSystemTest, Booking_FullFlow)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ensurePatientLogin(system);

    bool booked = system.bookAppointment(doctorId, "2026", "10AM");
    EXPECT_TRUE(booked);

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(HospitalSystemTest, CancelFlow_AllBranches)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    int doctorId = getDoctorId(system);

    ensurePatientLogin(system);
    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    int apptId = apps[0].get_AppointmentId();

    EXPECT_TRUE(system.cancelAppointmentPatient(apptId));
}

TEST(HospitalSystemTest, ViewMyAppointments_Branches)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    int doctorId = getDoctorId(system);

    ensurePatientLogin(system);
    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(HospitalSystemTest, DoctorSchedule_And_Complete)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    int doctorId = getDoctorId(system);

    ensurePatientLogin(system);
    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    ASSERT_TRUE(system.login("doc@mail.com", "default"));

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    int apptId = schedule[0].get_AppointmentId();

    EXPECT_TRUE(system.completeAppointmentDoctor(apptId));
}

TEST(HospitalSystemTest, AdminViewAll)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("admin@mail.com", "admin123");

    auto users = system.adminViewAllUsers();
    auto apps = system.adminViewAllAppointments();

    EXPECT_GE(users.size(), 3);
    EXPECT_EQ(apps.size(), 0);
}
