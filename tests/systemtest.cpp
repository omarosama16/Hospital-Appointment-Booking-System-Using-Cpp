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
    for (auto *u : sys.adminViewAllUsers())
        if (u && u->get_role() == "doctor")
            return u->get_id();
    return -1;
}

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
}

/* ================= LOGIN FULL CONDITIONS ================= */

TEST(SysCov, Login_AllBranches)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("x", "y"));   // no match
    EXPECT_FALSE(sys.login("", ""));     // empty

    setupPatient(sys);
    EXPECT_TRUE(sys.login("p@mail.com", "123")); // success

    EXPECT_FALSE(sys.login("p@mail.com", "wrong")); // wrong password
}

/* ================= BOOKING CONDITIONS ================= */

TEST(SysCov, Booking_AllConditions)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM")); // no login

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(-1, "", "")); // invalid doctor

    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM")); // not found

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM")); // success
}

/* ================= CANCEL CONDITIONS ================= */

TEST(SysCov, Cancel_AllBranches)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.cancelAppointmentPatient(1)); // no user

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999)); // not found

    int docId = getDoctorId(sys);
    int id = sys.bookAppointment(docId, "2026", "10AM"),
        sys.viewMyAppointments().back().get_AppointmentId();

    // wrong patient scenario
    sys.registerNewPatient("p2","p2@mail.com","456","011");
    sys.login("p2@mail.com","456");
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));

    // correct cancel
    sys.login("p@mail.com","123");
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));

    // cancel again → already cancelled
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE CONDITIONS ================= */

TEST(SysCov, Complete_AllBranches)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.completeAppointmentDoctor(1)); // no user

    sys.login("p@mail.com","123");
    int docId = getDoctorId(sys);
    int id = sys.bookAppointment(docId,"2026","10AM"),
        sys.viewMyAppointments().back().get_AppointmentId();

    // patient trying complete
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));

    sys.login("d@mail.com","123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id)); // success
    EXPECT_FALSE(sys.completeAppointmentDoctor(id)); // already completed

    // cancelled case
    sys.login("p@mail.com","123");
    int id2 = sys.bookAppointment(docId,"2026","11AM"),
        sys.viewMyAppointments().back().get_AppointmentId();

    sys.cancelAppointmentPatient(id2);

    sys.login("d@mail.com","123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id2));
}

/* ================= VIEW CONDITIONS ================= */

TEST(SysCov, Views_AllBranches)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com","123");
    int docId = getDoctorId(sys);
    sys.bookAppointment(docId,"2026","10AM");

    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);

    sys.login("d@mail.com","123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
}

/* ================= FIND DOCTOR CONDITIONS ================= */

TEST(SysCov, FindDoctor_Branches)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com","123");

    EXPECT_FALSE(sys.bookAppointment(9999,"2026","10AM")); // not found
}
