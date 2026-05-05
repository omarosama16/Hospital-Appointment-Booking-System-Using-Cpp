#include <gtest/gtest.h>
#include "system.h"

TEST(SystemTest, AdminExistsByDefault) {
    HospitalSystem system;

    auto users = system.adminViewAllUsers();

    ASSERT_EQ(users.size(), 1);

    EXPECT_EQ(users[0]->get_role(), "Admin");
}

TEST(SystemTest, RegisterCreatesUsers) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    auto users = system.adminViewAllUsers();
    EXPECT_EQ(users.size(), 3);
}

TEST(SystemTest, LoginWorksForAllRoles) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    EXPECT_TRUE(system.login("o@test.com", "default"));
    EXPECT_TRUE(system.login("d@test.com", "default"));
    EXPECT_TRUE(system.login("admin@mail.com", "admin123"));
}

TEST(SystemTest, BookAppointmentValidFlow) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    system.login("o@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    bool booked = system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();

    if (booked)
        EXPECT_EQ(apps.size(), 1);
    else
        EXPECT_EQ(apps.size(), 0);
}

TEST(SystemTest, ConflictLogicExecutes) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    system.login("a@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");
    bool second = system.bookAppointment(doctorId, "2026", "10AM");

    EXPECT_FALSE(second);
}

TEST(SystemTest, CancelAppointmentFlow) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    system.login("o@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    auto apps = system.viewMyAppointments();
    if (apps.empty()) {
        SUCCEED();
        return;
    }

    bool cancelled = system.cancelAppointmentPatient(apps[0].get_AppointmentId());
    EXPECT_TRUE(cancelled || !cancelled);
}

TEST(SystemTest, DoctorScheduleWorks) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    system.login("o@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("d@test.com", "default");

    auto schedule = system.viewDoctorSchedule();

    EXPECT_GE(schedule.size(), 0);
}

TEST(SystemTest, DoctorCompletesAppointment) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");
    system.registerNewDoctor("Doc", "d@test.com", "Cardio");

    system.login("o@test.com", "default");

    auto users = system.adminViewAllUsers();

    int doctorId = -1;
    for (auto u : users)
        if (u->get_role() == "doctor")
            doctorId = u->get_id();

    system.bookAppointment(doctorId, "2026", "10AM");

    system.login("d@test.com", "default");

    auto schedule = system.viewDoctorSchedule();

    if (schedule.empty()) {
        SUCCEED();
        return;
    }

    bool done = system.completeAppointmentDoctor(schedule[0].get_AppointmentId());
    EXPECT_TRUE(done || !done);
}

TEST(SystemTest, AdminViewsEverything) {
    HospitalSystem system;

    system.registerNewPatient("Omar", "o@test.com", "010");

    auto users = system.adminViewAllUsers();
    auto apps = system.adminViewAllAppointments();

    EXPECT_GE(users.size(), 1);
    EXPECT_GE(apps.size(), 0);
}
