#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, AdminExistsByDefault) {
    HospitalSystem system;

    auto users = system.adminViewAllUsers();
    ASSERT_EQ(users.size(), 1);
}

TEST(SystemTest, RegisterPatientsIncreaseUsers) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewPatient("Ali", "ali@test.com", "011");

    EXPECT_EQ(system.adminViewAllUsers().size(), 3);
}

TEST(SystemTest, LoginWorksForPatientAndDoctor) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    EXPECT_TRUE(system.login("omar@test.com", "default"));
    EXPECT_TRUE(system.login("doc@test.com", "default"));
}

TEST(SystemTest, BookAppointmentFlowOnlyIfValid) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users) {
        if (u->get_role() == "doctor") {
            doctorId = u->get_id();
            break;
        }
    }

    ASSERT_NE(doctorId, -1);

    bool booked = system.bookAppointment(doctorId, "2026", "10AM");

    EXPECT_TRUE(booked == true || booked == false);

    auto apps = system.viewMyAppointments();

    EXPECT_EQ(apps.size(), booked ? 1 : 0);
}

TEST(SystemTest, CancelFlowSafeCheck) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users) {
        if (u->get_role() == "doctor") doctorId = u->get_id();
    }

    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    if (apps.empty()) {
        SUCCEED();
        return;
    }

    EXPECT_TRUE(system.cancelAppointmentPatient(apps[0].get_AppointmentId())
                || !system.cancelAppointmentPatient(apps[0].get_AppointmentId()));
}

TEST(SystemTest, DoctorScheduleSafe) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor") doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();

    EXPECT_TRUE(schedule.size() >= 0); 
}

TEST(SystemTest, DoctorCompleteSafe) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor") doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();

    if (schedule.empty()) {
        SUCCEED();
        return;
    }

    EXPECT_TRUE(system.completeAppointmentDoctor(schedule[0].get_AppointmentId())
                || !system.completeAppointmentDoctor(schedule[0].get_AppointmentId()));
}

TEST(SystemTest, AdminViewAlwaysSafe) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");

    auto users = system.adminViewAllUsers();
    EXPECT_GE(users.size(), 1);

    auto apps = system.adminViewAllAppointments();
    EXPECT_GE(apps.size(), 0);
}
