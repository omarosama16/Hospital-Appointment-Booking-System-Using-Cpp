#include <gtest/gtest.h>
#include "system.h"

class SysTest : public HospitalSystem
{
public:
    using HospitalSystem::adminViewAllAppointments;
    using HospitalSystem::adminViewAllUsers;
};

static int docId(HospitalSystem &sys)
{
    for (auto u : sys.adminViewAllUsers())
        if (u && u->get_role() == "doctor")
            return u->get_id();

    return -1;
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
    sys.login("p@mail.com", "123");
}

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
}

/* ================= AUTH ================= */

TEST(Sys, Login)
{
    HospitalSystem sys;

    setupPatient(sys);
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
    EXPECT_FALSE(sys.login("x@mail.com", "123"));
}

/* ================= BOOKING ================= */

TEST(Sys, Booking)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    int id = docId(sys);

    EXPECT_TRUE(sys.bookAppointment(id, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

/* ================= AUTH RULES ================= */

TEST(Sys, BookingRules)
{
    HospitalSystem sys;

    setupDoctor(sys);

    int id = docId(sys);

    EXPECT_FALSE(sys.bookAppointment(id, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(Sys, Cancel)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    int id = docId(sys);

    sys.bookAppointment(id, "2026", "10AM");
    int appt = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(appt));
    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

/* ================= COMPLETE ================= */

TEST(Sys, Complete)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    int id = docId(sys);

    sys.bookAppointment(id, "2026", "10AM");
    int appt = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(appt));
    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

/* ================= VIEW ================= */

TEST(Sys, Views)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());

    setupDoctor(sys);
    setupPatient(sys);

    int id = docId(sys);

    sys.bookAppointment(id, "2026", "10AM");

    EXPECT_FALSE(sys.viewMyAppointments().empty());
    EXPECT_FALSE(sys.viewDoctorSchedule().empty());
}

/* ================= ADMIN ================= */

TEST(Sys, Admin)
{
    SysTest sys;

    EXPECT_FALSE(sys.adminViewAllUsers().empty());
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}