#include <gtest/gtest.h>
#include "system.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"

TEST(HospitalSystemTest, Login_AllBranches)
{
    HospitalSystem system;

    // wrong login
    EXPECT_FALSE(system.login("wrong@mail.com", "123"));

    // correct admin login (created in constructor)
    EXPECT_TRUE(system.login("admin@mail.com", "admin123"));
}

TEST(HospitalSystemTest, AdminAndUserCreation)
{
    HospitalSystem system;

    system.registerNewPatient("Ali", "ali@mail.com", "0100");
    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");

    system.login("admin@mail.com", "admin123");

    auto users = system.adminViewAllUsers();
    EXPECT_GE(users.size(), 3); // admin + patient + doctor
}

TEST(HospitalSystemTest, Booking_FullFlow)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("ali@mail.com", "default"); // patient login

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    bool booked = system.bookAppointment(doctorId, "2026", "10AM");
    EXPECT_TRUE(booked);

    auto myApps = system.viewMyAppointments();
    EXPECT_EQ(myApps.size(), 1);
}

TEST(HospitalSystemTest, CancelFlow_AllBranches)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("ali@mail.com", "default");

    int doctorId = system.adminViewAllUsers()[1]->get_id();

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    int apptId = apps[0].get_AppointmentId();

    EXPECT_TRUE(system.cancelAppointmentPatient(apptId));

    auto updated = system.viewMyAppointments();
    EXPECT_EQ(updated.size(), 1); // still exists but cancelled
}

TEST(HospitalSystemTest, ViewMyAppointments_Branches)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("ali@mail.com", "default");

    int doctorId = system.adminViewAllUsers()[1]->get_id();

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(HospitalSystemTest, DoctorSchedule_And_Complete)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("ali@mail.com", "default");

    int doctorId = system.adminViewAllUsers()[1]->get_id();

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("doc@mail.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    int apptId = schedule[0].get_AppointmentId();

    EXPECT_TRUE(system.completeAppointmentDoctor(apptId));

    auto updated = system.viewDoctorSchedule();
    EXPECT_EQ(updated.size(), 1);
}

TEST(HospitalSystemTest, AdminViewAll)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "0100");

    system.login("admin@mail.com", "admin123");

    auto allAppts = system.adminViewAllAppointments();
    auto allUsers = system.adminViewAllUsers();

    EXPECT_GE(allUsers.size(), 3);
    EXPECT_EQ(allAppts.size(), 0); // no booking yet
}
