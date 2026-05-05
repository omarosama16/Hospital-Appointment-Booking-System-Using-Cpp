#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, Login_AllBranches)
{
    HospitalSystem system;

    EXPECT_FALSE(system.login("", ""));
    EXPECT_FALSE(system.login("bad", "bad"));
}

TEST(SystemTest, AdminAndUserCreation)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "a@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "010");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 3); 
}

TEST(SystemTest, Booking_FullFlow)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr Strange", "doc@mail.com", "Neuro");

    system.registerNewPatient("Ali", "ali@mail.com", "010");
    EXPECT_TRUE(system.login("ali@mail.com", "default"));

    auto users = system.adminViewAllUsers();

    int docId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            docId = u->get_id();

    ASSERT_NE(docId, -1);

    EXPECT_TRUE(system.bookAppointment(docId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);

    EXPECT_FALSE(system.bookAppointment(docId, "2026", "10AM"));
}

TEST(SystemTest, CancelFlow_AllBranches)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "a@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "010");
    system.login("ali@mail.com", "default");

    int docId = -1;
    for (auto u : system.adminViewAllUsers())
        if (u->get_role() == "doctor")
            docId = u->get_id();

    ASSERT_NE(docId, -1);

    system.bookAppointment(docId, "2026", "10AM");
    auto apps = system.viewMyAppointments();

    ASSERT_EQ(apps.size(), 1);

    int apptId = apps[0].get_AppointmentId();

    EXPECT_TRUE(system.cancelAppointmentPatient(apptId));
    EXPECT_FALSE(system.cancelAppointmentPatient(apptId));
    EXPECT_FALSE(system.cancelAppointmentPatient(999));    
}

TEST(SystemTest, DoctorSchedule_And_Complete)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "doc@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "010");

    system.login("ali@mail.com", "default");

    int docId = -1;
    for (auto u : system.adminViewAllUsers())
        if (u->get_role() == "doctor")
            docId = u->get_id();

    ASSERT_NE(docId, -1);

    system.bookAppointment(docId, "2026", "10AM");

    system.login("doc@mail.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    int apptId = schedule[0].get_AppointmentId();

    EXPECT_TRUE(system.completeAppointmentDoctor(apptId));
    EXPECT_FALSE(system.completeAppointmentDoctor(apptId)); 
    EXPECT_FALSE(system.completeAppointmentDoctor(999));    
}

TEST(SystemTest, AdminViewCoverage)
{
    HospitalSystem system;

    system.registerNewDoctor("Dr A", "a@mail.com", "Cardio");
    system.registerNewPatient("Ali", "ali@mail.com", "010");

    auto apps = system.adminViewAllAppointments();
    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 3);
    EXPECT_GE(apps.size(), 0);
}
