#include <gtest/gtest.h>
#include "system.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"

// ---------- Helper test-only subclass ----------
class TestUserSystem : public HospitalSystem {
public:
    using HospitalSystem::adminViewAllUsers;
    using HospitalSystem::adminViewAllAppointments;
};

// ---------------- SAFE HELPER ----------------
// dynamically fetch doctor ID instead of assuming "2"
static int getDoctorId(HospitalSystem& system)
{
    auto users = system.adminViewAllUsers();

    for (auto &u : users)
    {
        if (u.getRole() == "doctor")
        {
            return u.getId();
        }
    }
    return -1;
}

// -------------------- SYSTEM TESTS --------------------

TEST(HospitalSystemTest, Login_Success_And_Failure)
{
    HospitalSystem system;

    EXPECT_TRUE(system.login("admin@mail.com", "admin123"));
    EXPECT_FALSE(system.login("wrong@mail.com", "wrong"));
}

TEST(HospitalSystemTest, Create_Doctor_And_Patient)
{
    TestUserSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    auto users = system.adminViewAllUsers();

    EXPECT_GE(users.size(), 2);
}

// -------------------- BOOKING --------------------

TEST(HospitalSystemTest, Booking_Success)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    bool booked = system.bookAppointment(doctorId, "2026", "10AM");

    EXPECT_TRUE(booked);

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

TEST(HospitalSystemTest, Booking_Fails_NotLoggedIn)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");

    int doctorId = getDoctorId(system);
    if (doctorId != -1)
    {
        EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
    }
}

TEST(HospitalSystemTest, Booking_Fails_WrongRole)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.login("admin@mail.com", "admin123");

    int doctorId = getDoctorId(system);
    if (doctorId != -1)
    {
        EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
    }
}

TEST(HospitalSystemTest, Booking_Fails_InvalidDoctor)
{
    HospitalSystem system;

    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    EXPECT_FALSE(system.bookAppointment(9999, "2026", "10AM"));
}

TEST(HospitalSystemTest, Booking_Fails_Conflict)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    EXPECT_FALSE(system.bookAppointment(doctorId, "2026", "10AM"));
}

// -------------------- VIEW APPOINTMENTS --------------------

TEST(HospitalSystemTest, ViewMyAppointments_Empty_And_Filled)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    EXPECT_TRUE(system.viewMyAppointments().empty());

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.viewMyAppointments();
    EXPECT_EQ(apps.size(), 1);
}

// -------------------- DOCTOR TESTS --------------------

TEST(HospitalSystemTest, DoctorSchedule_And_Complete_Success)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    ASSERT_TRUE(system.login("doc@mail.com", "123"));

    auto schedule = system.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1);

    EXPECT_TRUE(system.completeAppointmentDoctor(1));
}

TEST(HospitalSystemTest, DoctorSchedule_Fails_NotDoctor)
{
    HospitalSystem system;

    system.login("admin@mail.com", "admin123");

    EXPECT_TRUE(system.viewDoctorSchedule().empty());
}

TEST(HospitalSystemTest, Complete_Fails_WrongDoctor)
{
    HospitalSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewDoctor("doc2", "doc2@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    ASSERT_TRUE(system.login("doc2@mail.com", "123"));

    EXPECT_FALSE(system.completeAppointmentDoctor(1));
}

// -------------------- ADMIN TESTS --------------------

TEST(HospitalSystemTest, Admin_View_All)
{
    TestUserSystem system;

    system.registerNewDoctor("doc1", "doc@mail.com", "cardio");
    system.registerNewPatient("p1", "p@mail.com", "123");

    ASSERT_TRUE(system.login("p@mail.com", "123"));

    int doctorId = getDoctorId(system);
    ASSERT_NE(doctorId, -1);

    ASSERT_TRUE(system.bookAppointment(doctorId, "2026", "10AM"));

    auto apps = system.adminViewAllAppointments();
    auto users = system.adminViewAllUsers();

    EXPECT_EQ(apps.size(), 1);
    EXPECT_GE(users.size(), 2);
}
