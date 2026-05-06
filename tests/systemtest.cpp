#include <gtest/gtest.h>
#include "system.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"

class TestSystem : public HospitalSystem {
public:
    using HospitalSystem::adminViewAllUsers;
    using HospitalSystem::adminViewAllAppointments;
};

// ---------------- HELPERS ----------------

static int getDoctorId(HospitalSystem& system)
{
    auto users = system.adminViewAllUsers();

    for (auto &u : users)
    {
        if (u.getRole() == "doctor")
            return u.getId();
    }
    return -1;
}

// ---------------- LOGIN ----------------

TEST(HospitalSystem_Safe, Login_Success_Failure)
{
    HospitalSystem system;

    EXPECT_TRUE(system.login("admin@mail.com", "admin123"));
    EXPECT_FALSE(system.login("invalid@mail.com", "wrong"));
}

// ---------------- USER CREATION ----------------

TEST(HospitalSystem_Safe, Register_Users)
{
    TestSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 2);
}

// ---------------- BOOKING (SAFE FLOW) ----------------

TEST(HospitalSystem_Safe, Booking_Full_Flow)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    EXPECT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

// ---------------- BOOKING FAILURES ----------------

TEST(HospitalSystem_Safe, Booking_NotLoggedIn)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");

    int doctorId = 1; // safe fallback (system dependent)
    EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
}

TEST(HospitalSystem_Safe, Booking_InvalidDoctor)
{
    HospitalSystem system;

    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    EXPECT_FALSE(system.bookAppointment(9999, "2026", "10AM"));
}

TEST(HospitalSystem_Safe, Booking_Conflict)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    EXPECT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));
    EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
}

// ---------------- VIEW APPOINTMENTS ----------------

TEST(HospitalSystem_Safe, ViewAppointments)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    EXPECT_TRUE(system.viewMyAppointments().empty());

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    EXPECT_EQ(system.viewMyAppointments().size(), 1);
}

// ---------------- DOCTOR FLOW ----------------

TEST(HospitalSystem_Safe, Doctor_Flow)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    ASSERT_TRUE(system.login("doc@mail.com", "123"));

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    EXPECT_TRUE(system.completeAppointmentDoctor(1));
}

// ---------------- ADMIN ----------------

TEST(HospitalSystem_Safe, Admin_View)
{
    TestSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    system.bookAppointment(doctorId, "2026", "10AM");

    auto users = system.adminViewAllUsers();
    auto apps = system.adminViewAllAppointments();

    EXPECT_GE(users.size(), 2);
    EXPECT_EQ(apps.size(), 1);
}
