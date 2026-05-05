#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, LoginFailsWithoutUsers) {
    HospitalSystem system;
    EXPECT_FALSE(system.login("none@test.com","1234"));
}

TEST(SystemTest, RegisterMultipleUsers) {
    HospitalSystem system;

    system.registerNewPatient("Omar","omar@test.com","010");
    system.registerNewPatient("Ali","ali@test.com","011");

    auto users = system.adminViewAllUsers();
    EXPECT_EQ(users.size(), 2);
}

TEST(SystemTest, BookWithoutLoginFails) {
    HospitalSystem system;

    system.registerNewDoctor("Doc","doc@test.com","Cardio");

    EXPECT_FALSE(system.bookAppointment(1,"2026","10:00"));
}

TEST(SystemTest, CancelNonExistingAppointment) {
    HospitalSystem system;

    system.registerNewPatient("Omar","omar@test.com","010");
    system.login("omar@test.com","1234");

    EXPECT_FALSE(system.cancelAppointmentPatient(999));
}

TEST(SystemTest, ViewAppointmentsEmpty) {
    HospitalSystem system;

    system.registerNewPatient("Omar","omar@test.com","010");
    system.login("omar@test.com","1234");

    auto apps = system.viewMyAppointments();
    EXPECT_TRUE(apps.empty());
}

TEST(SystemTest, FullFlow) {
    HospitalSystem system;

    system.registerNewPatient("Omar","omar@test.com","010");
    system.registerNewDoctor("Doc","doc@test.com","Cardio");

    system.login("omar@test.com","1234");

    EXPECT_TRUE(system.bookAppointment(2,"2026-05-20","10:00"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);

    EXPECT_TRUE(system.cancelAppointmentPatient(apps[0].get_AppointmentId()));
}

TEST(SystemTest, DoctorCompleteFlow) {
    HospitalSystem system;

    system.registerNewPatient("Omar","omar@test.com","010");
    system.registerNewDoctor("Doc","doc@test.com","Cardio");

    system.login("omar@test.com","1234");
    system.bookAppointment(2,"2026","10:00");

    system.login("doc@test.com","1234");

    auto schedule = system.viewDoctorSchedule();
    ASSERT_FALSE(schedule.empty());

    EXPECT_TRUE(system.completeAppointmentDoctor(schedule[0].get_AppointmentId()));
}
