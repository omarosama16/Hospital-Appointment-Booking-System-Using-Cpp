#include <gtest/gtest.h>
#include <algorithm>

#include "system.h"
#include "user.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"
#include "Appointment.h"

// ══════════════════════════════════════════════════════════════
// HELPERS (SAFE)
// ══════════════════════════════════════════════════════════════

static User* findUserByRole(const std::vector<User*>& users, const std::string& role) {
    for (auto* u : users) {
        if (u && u->get_role() == role) return u;
    }
    return nullptr;
}

static std::vector<User*> getUsers(HospitalSystem& sys) {
    return sys.adminViewAllUsers();
}

static int getDoctorId(HospitalSystem& sys) {
    auto users = getUsers(sys);
    auto* d = findUserByRole(users, "doctor");
    return d ? d->get_id() : -1;
}

static int getPatientId(HospitalSystem& sys) {
    auto users = getUsers(sys);
    for (auto* u : users) {
        if (u && u->get_role() == "patient") return u->get_id();
    }
    return -1;
}

static void setupDoctorPatient(HospitalSystem& sys) {
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "010");
}

// ══════════════════════════════════════════════════════════════
// APPOINTMENT
// ══════════════════════════════════════════════════════════════

TEST(AppointmentTest, BasicFlow) {
    Appointment a(1, 10, 20, "Alice", "Dr", "2025", "10", "Scheduled");

    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_Status(), "Scheduled");

    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");

    a.complete();
    EXPECT_EQ(a.get_Status(), "Completed");
}

// ══════════════════════════════════════════════════════════════
// USER AUTH
// ══════════════════════════════════════════════════════════════

TEST(UserTest, Auth) {
    Admin u(1, "admin", "admin@mail.com", "admin123");

    EXPECT_TRUE(u.Authenticate("admin@mail.com", "admin123"));
    EXPECT_FALSE(u.Authenticate("admin@mail.com", "wrong"));
    EXPECT_FALSE(u.Authenticate("x@mail.com", "admin123"));
}

// ══════════════════════════════════════════════════════════════
// DOCTOR / PATIENT
// ══════════════════════════════════════════════════════════════

TEST(DoctorPatientTest, RolesAndData) {
    Doctor d(1, "Dr", "d@mail.com", "p", "Cardio");
    patient p(2, "Alice", "a@mail.com", "p", "010");

    EXPECT_EQ(d.get_role(), "doctor");
    EXPECT_EQ(p.get_role(), "patient");

    d.addAvailability("10AM");
    auto slots = d.getAvailability();

    EXPECT_FALSE(slots.empty());
    EXPECT_NE(std::find(slots.begin(), slots.end(), "10AM"), slots.end());
}

// ══════════════════════════════════════════════════════════════
// SYSTEM BASICS
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, ConstructorSafe) {
    HospitalSystem sys;

    auto users = getUsers(sys);
    ASSERT_FALSE(users.empty());

    EXPECT_EQ(users[0]->get_role(), "admin");
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}

TEST(SystemTest, LoginFlow) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_FALSE(sys.login("wrong", "wrong"));

    EXPECT_TRUE(sys.login("alice@mail.com", "default"));
    EXPECT_TRUE(sys.login("smith@mail.com", "default"));
}

// ══════════════════════════════════════════════════════════════
// REGISTRATION
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, RegisterUsers) {
    HospitalSystem sys;

    sys.registerNewDoctor("Dr", "d@mail.com", "Cardio");
    sys.registerNewPatient("Alice", "a@mail.com", "010");

    auto users = getUsers(sys);
    EXPECT_GE(users.size(), 3u);

    EXPECT_NE(getDoctorId(sys), -1);
    EXPECT_NE(getPatientId(sys), -1);
}

// ══════════════════════════════════════════════════════════════
// BOOKING
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, BookingFlow) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_FALSE(sys.bookAppointment(docId, "d", "t")); // not logged

    sys.login("alice@mail.com", "default");

    EXPECT_TRUE(sys.bookAppointment(docId, "2025", "10"));
    EXPECT_FALSE(sys.bookAppointment(docId, "2025", "10")); // conflict

    auto appts = sys.adminViewAllAppointments();
    ASSERT_EQ(appts.size(), 1u);

    EXPECT_EQ(appts[0].get_Status(), "Scheduled");
}

// ══════════════════════════════════════════════════════════════
// CANCEL
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, CancelFlow) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);
    sys.login("alice@mail.com", "default");

    sys.bookAppointment(docId, "2025", "10");

    EXPECT_TRUE(sys.cancelAppointmentPatient(1));
    EXPECT_FALSE(sys.cancelAppointmentPatient(1)); // already cancelled
}

// ══════════════════════════════════════════════════════════════
// VIEW PATIENT
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, ViewPatientAppointments) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);
    sys.login("alice@mail.com", "default");

    sys.bookAppointment(docId, "2025", "10");

    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
}

// ══════════════════════════════════════════════════════════════
// VIEW DOCTOR
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, ViewDoctorSchedule) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);

    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025", "10");

    sys.login("smith@mail.com", "default");

    auto sched = sys.viewDoctorSchedule();
    EXPECT_EQ(sched.size(), 1u);
}

// ══════════════════════════════════════════════════════════════
// COMPLETE
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, CompleteFlow) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);

    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025", "10");

    sys.login("smith@mail.com", "default");

    EXPECT_TRUE(sys.completeAppointmentDoctor(1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(1)); // already done
}

// ══════════════════════════════════════════════════════════════
// ADMIN VIEWS
// ══════════════════════════════════════════════════════════════

TEST(SystemTest, AdminViews) {
    HospitalSystem sys;
    setupDoctorPatient(sys);

    int docId = getDoctorId(sys);

    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025", "10");

    auto appts = sys.adminViewAllAppointments();
    EXPECT_FALSE(appts.empty());

    auto users = sys.adminViewAllUsers();
    EXPECT_FALSE(users.empty());
}
