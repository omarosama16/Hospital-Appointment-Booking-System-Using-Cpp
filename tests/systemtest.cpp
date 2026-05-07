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

/* ================= LOGIN ================= */

TEST(HospitalSystemTest, LoginSuccess)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(HospitalSystemTest, LoginFail)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("x", "y"));
}

/* ================= BOOKING ================= */

TEST(HospitalSystemTest, BookSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookWithoutLogin)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookWrongRole)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookInvalidDoctor)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(HospitalSystemTest, CancelSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

TEST(HospitalSystemTest, CancelInvalid)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

/* ================= COMPLETE ================= */

TEST(HospitalSystemTest, CompleteSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

TEST(HospitalSystemTest, CompleteInvalid)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

/* ================= VIEW ================= */

TEST(HospitalSystemTest, ViewEmptyPatient)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(HospitalSystemTest, ViewEmptyDoctor)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(HospitalSystemTest, ViewWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= EDGE CASES ================= */

TEST(HospitalSystemTest, WrongRoleCannotBook)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, PatientCannotCompleteOthers)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    // patient tries to complete
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= ADMIN ================= */

TEST(HospitalSystemTest, AdminDataAccess)
{
    TestSystem sys;

    EXPECT_FALSE(sys.adminViewAllUsers().empty());
}