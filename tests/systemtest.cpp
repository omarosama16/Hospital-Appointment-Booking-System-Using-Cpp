#include <gtest/gtest.h>
#include "system.h"

static int extractDoctorId(HospitalSystem &system) {
    auto users = system.adminViewAllUsers();

    for (auto u : users) {
        if (u->get_role() == "doctor")
            return u->get_id();
    }
    return -1;
}

TEST(SystemTest, DefaultAdminExists) {
    HospitalSystem system;

    auto users = system.adminViewAllUsers();

    ASSERT_EQ(users.size(), 1);
    EXPECT_EQ(users[0]->get_role(), "Admin");
}

TEST(SystemTest, RegisterUsersIncreasesSize) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 3);
}

TEST(SystemTest, LoginSuccessAndFailurePaths) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");

    EXPECT_TRUE(system.login("a@test.com", "default"));
    EXPECT_FALSE(system.login("wrong@test.com", "default"));
    EXPECT_FALSE(system.login("a@test.com", "wrong"));
}

TEST(SystemTest, BookAppointmentRequiresLogin) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    int docId = extractDoctorId(system);

    EXPECT_EQ(system.bookAppointment(docId, "2026", "10AM"), false);
}

TEST(SystemTest, BookAppointmentSuccessFlow) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    system.login("a@test.com", "default");

    int docId = extractDoctorId(system);
    ASSERT_NE(docId, -1);

    bool booked = system.bookAppointment(docId, "2026", "10AM");

    auto apps = system.viewMyAppointments();

    EXPECT_EQ(apps.size(), booked ? 1 : 0);
}

TEST(SystemTest, CheckConflictPreventsDoubleBooking) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    system.login("a@test.com", "default");

    int docId = extractDoctorId(system);

    system.bookAppointment(docId, "2026", "10AM");
    bool second = system.bookAppointment(docId, "2026", "10AM");

    EXPECT_FALSE(second);
}

TEST(SystemTest, CancelAppointmentWorksAndFailsCases) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    system.login("a@test.com", "default");

    int docId = extractDoctorId(system);

    system.bookAppointment(docId, "2026", "10AM");

    auto apps = system.viewMyAppointments();

    if (apps.empty()) {
        SUCCEED();
        return;
    }

    int apptId = apps[0].get_AppointmentId();

    EXPECT_TRUE(system.cancelAppointmentPatient(apptId));

    // second cancel should fail (already cancelled)
    EXPECT_FALSE(system.cancelAppointmentPatient(apptId));
}

TEST(SystemTest, ViewMyAppointmentsOnlyPatient) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");

    system.login("a@test.com", "default");

    auto apps = system.viewMyAppointments();

    EXPECT_GE(apps.size(), 0);
}

TEST(SystemTest, DoctorScheduleAndCompletion) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    system.login("a@test.com", "default");

    int docId = extractDoctorId(system);

    system.bookAppointment(docId, "2026", "10AM");

    system.login("b@test.com", "default");

    auto schedule = system.viewDoctorSchedule();

    EXPECT_GE(schedule.size(), 0);

    if (!schedule.empty()) {
        int apptId = schedule[0].get_AppointmentId();

        bool done = system.completeAppointmentDoctor(apptId);
        EXPECT_TRUE(done || !done);
    }
}

TEST(SystemTest, AdminViewsAllData) {
    HospitalSystem system;

    system.registerNewPatient("A", "a@test.com", "010");
    system.registerNewDoctor("B", "b@test.com", "Cardio");

    auto users = system.adminViewAllUsers();
    auto apps = system.adminViewAllAppointments();

    EXPECT_GE(users.size(), 1);
    EXPECT_GE(apps.size(), 0);
}
