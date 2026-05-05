#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, AdminExistsByDefault) {
    HospitalSystem system;

    auto users = system.adminViewAllUsers();

    ASSERT_EQ(users.size(), 1);
    EXPECT_EQ(users[0]->get_name(), "admin");
}

TEST(SystemTest, RegisterPatientsIncreaseUsers) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewPatient("Ali", "ali@test.com", "011");

    auto users = system.adminViewAllUsers();

    EXPECT_EQ(users.size(), 3); // admin + 2 patients
}

TEST(SystemTest, PatientAndDoctorLogin) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    EXPECT_TRUE(system.login("omar@test.com", "default"));
    EXPECT_TRUE(system.login("doc@test.com", "default"));
}

TEST(SystemTest, BookAppointmentSuccess_DynamicDoctorId) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    // find doctor dynamically instead of assuming ID = 3
    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users) {
        if (u->get_role() == "doctor") {
            doctorId = u->get_id();
            break;
        }
    }

    ASSERT_NE(doctorId, -1);

    EXPECT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(SystemTest, ConflictPreventsDoubleBooking) {
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

    EXPECT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));
    EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
}

TEST(SystemTest, CancelAppointmentWorks) {
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

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    EXPECT_TRUE(system.cancelAppointmentPatient(apps[0].get_AppointmentId()));
}

TEST(SystemTest, DoctorViewsSchedule) {
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

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);
}

TEST(SystemTest, DoctorCompletesAppointment) {
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

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();
    ASSERT_EQ(schedule.size(), 1);

    EXPECT_TRUE(system.completeAppointmentDoctor(schedule[0].get_AppointmentId()));
}

TEST(SystemTest, AdminSeesAppointments) {
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

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.adminViewAllAppointments();
    EXPECT_EQ(apps.size(), 1);
}
