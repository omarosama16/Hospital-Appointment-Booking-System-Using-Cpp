#include <gtest/gtest.h>
#include "system.h"

class SysTest : public HospitalSystem
{
public:
    using HospitalSystem::adminViewAllAppointments;
    using HospitalSystem::adminViewAllUsers;
};

/* ================= HELPERS ================= */

static int getDoctorId(HospitalSystem &sys)
{
    for (auto u : sys.adminViewAllUsers())
    {
        if (u && u->get_role() == "doctor")
            return u->get_id();
    }
    return -1;
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
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

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

/* ================= RULES ================= */

TEST(Sys, BookingRules)
{
    HospitalSystem sys;

    setupDoctor(sys);

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(Sys, Cancel)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(apptId));
    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

/* ================= COMPLETE ================= */

TEST(Sys, Complete)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

/* ================= VIEWS ================= */

TEST(Sys, Views)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_TRUE(sys.viewDoctorSchedule().empty());

    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");

    EXPECT_FALSE(sys.viewMyAppointments().empty());

    sys.login("d@mail.com", "123");

    auto doctorView = sys.viewDoctorSchedule();
    EXPECT_FALSE(doctorView.empty());
    EXPECT_EQ(doctorView.size(), 1);

    // switch back
    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1);
}

/* ================= ADMIN ================= */

TEST(Sys, Admin)
{
    SysTest sys;

    EXPECT_FALSE(sys.adminViewAllUsers().empty());
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}