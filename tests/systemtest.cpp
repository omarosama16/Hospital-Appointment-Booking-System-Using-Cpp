#include <gtest/gtest.h>
#include "system.h"

class TestSystem : public HospitalSystem
{
public:
    using HospitalSystem::adminViewAllAppointments;
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

/* ================= EMPTY SYSTEM PATHS ================= */

TEST(SystemBoost, EmptySystemAllViews)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

/* ================= LOGIN EDGE ================= */

TEST(SystemBoost, LoginFailCases)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("x", "y"));
}

/* ================= ROLE BLOCKING ================= */

TEST(SystemBoost, RoleMismatchPatientCheck)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= BOOKING EDGE ================= */

TEST(SystemBoost, BookWithoutDoctorExists)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

/* ================= FULL FLOW ================= */

TEST(SystemBoost, FullFlowValid)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));

    auto appts = sys.viewMyAppointments();
    ASSERT_FALSE(appts.empty());

    int id = appts[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

/* ================= WRONG USER ACTIONS ================= */

TEST(SystemBoost, WrongUserCannotModify)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("admin@mail.com", "admin123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= DOCTOR FLOW ================= */

TEST(SystemBoost, DoctorCompletionFlow)
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