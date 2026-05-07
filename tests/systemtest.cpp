#include <gtest/gtest.h>
#include "system.h"

class TestSystem : public HospitalSystem
{
public:
    using HospitalSystem::adminViewAllUsers;
};

/* ================= HELPERS ================= */

static int getDoctorId(HospitalSystem &sys)
{
    auto users = sys.adminViewAllUsers();

    for (auto u : users)
        if (u && u->get_role() == "doctor")
            return u->get_id();

    return -1;
}

/* ================= LOGIN ================= */

TEST(Sys, Login)
{
    HospitalSystem sys;
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
    EXPECT_FALSE(sys.login("x", "y"));
}

/* ================= NULL USER ================= */

TEST(Sys, NullUser)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

/* ================= BOOKING ================= */

TEST(Sys, Booking)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(Sys, Cancel)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE ================= */

TEST(Sys, Complete)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* ================= VIEW ================= */

TEST(Sys, View)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= ROLE BLOCK ================= */

TEST(Sys, RoleBlock)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= FULL FLOW ================= */

TEST(Sys, FullFlow)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.cancelAppointmentPatient(id);

    sys.login("d@mail.com", "123");

    // FIXED: now consistent with logic
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}