#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, LoginFailsWithoutUsers) {
    HospitalSystem system;

    EXPECT_FALSE(system.login("fake@mail.com", "1234"));
}

TEST(SystemTest, RegisterMultipleUsers) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewPatient("Ali", "ali@test.com", "011");

    auto users = system.adminViewAllUsers();

    // 1 admin + 2 patients
    EXPECT_EQ(users.size(), 3);
}

TEST(SystemTest, PatientLoginSuccess) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");

    EXPECT_TRUE(system.login("omar@test.com", "default"));
}

TEST(SystemTest, DoctorLoginSuccess) {
    HospitalSystem system;

    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    EXPECT_TRUE(system.login("doc@test.com", "default"));
}

TEST(SystemTest, BookWithoutLoginFails) {
    HospitalSystem system;

    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    EXPECT_FALSE(system.bookAppointment(2, "2026", "10AM"));
}

TEST(SystemTest, BookWithWrongRoleFails) {
    HospitalSystem system;

    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");
    system.login("doc@test.com", "default"); // doctor logged in

    EXPECT_FALSE(system.bookAppointment(2, "2026", "10AM"));
}

TEST(SystemTest, BookAppointmentSuccess) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    // IDs:
    // 1 = admin
    // 2 = patient
    // 3 = doctor

    EXPECT_TRUE(system.bookAppointment(3, "2026-05-20", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(SystemTest, ConflictPreventsBooking) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    EXPECT_TRUE(system.bookAppointment(3, "2026", "10AM"));

    // same slot → should fail
    EXPECT_FALSE(system.bookAppointment(3, "2026", "10AM"));
}

TEST(SystemTest, CancelAppointmentSuccess) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");

    system.bookAppointment(3, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    EXPECT_TRUE(system.cancelAppointmentPatient(apps[0].get_AppointmentId()));
}

TEST(SystemTest, CancelWrongUserFails) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewPatient("Ali", "ali@test.com", "011");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");
    system.bookAppointment(4, "2026", "10AM"); // doctor id = 4

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    system.login("ali@test.com", "default");

    EXPECT_FALSE(system.cancelAppointmentPatient(apps[0].get_AppointmentId()));
}

TEST(SystemTest, DoctorViewSchedule) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");
    system.bookAppointment(3, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);
}

TEST(SystemTest, DoctorCompleteAppointment) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");
    system.bookAppointment(3, "2026", "10AM");

    system.login("doc@test.com", "default");

    auto schedule = system.viewDoctorSchedule();
    ASSERT_EQ(schedule.size(), 1);

    EXPECT_TRUE(system.completeAppointmentDoctor(schedule[0].get_AppointmentId()));
}

TEST(SystemTest, AdminViewAllAppointments) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "omar@test.com", "010");
    system.registerNewDoctor("Doc", "doc@test.com", "Cardio");

    system.login("omar@test.com", "default");
    system.bookAppointment(3, "2026", "10AM");

    auto apps = system.adminViewAllAppointments();
    EXPECT_EQ(apps.size(), 1);
}
