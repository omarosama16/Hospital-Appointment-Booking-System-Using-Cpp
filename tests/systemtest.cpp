#include <gtest/gtest.h>
#include "system.h"

class TestSystem : public HospitalSystem
{
public:
    using HospitalSystem::adminViewAllAppointments;
    using HospitalSystem::adminViewAllUsers;
};

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

TEST(HospitalSystemTest, LoginFailWrongPassword)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
}

TEST(HospitalSystemTest, LoginFailWrongEmail)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("wrong@mail.com", "123"));
}

/* ================= BOOKING ================= */

TEST(HospitalSystemTest, BookAppointmentSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentWithoutLogin)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentWrongRole)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentDoctorNotFound)
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

TEST(HospitalSystemTest, CancelInvalidId)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

TEST(HospitalSystemTest, CancelWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
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

TEST(HospitalSystemTest, CompleteInvalidId)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

TEST(HospitalSystemTest, CompleteWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

/* ================= VIEW ================= */

TEST(HospitalSystemTest, ViewMyAppointmentsEmpty)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(HospitalSystemTest, ViewDoctorScheduleEmpty)
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

TEST(HospitalSystemTest, WrongUserCannotCancelOthers)
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

TEST(HospitalSystemTest, FullFlowStress)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(id)); // cancelled case
}

/* ================= ADMIN ================= */

TEST(HospitalSystemTest, AdminViews)
{
    TestSystem sys;

    auto users = sys.adminViewAllUsers();
    auto appts = sys.adminViewAllAppointments();

    EXPECT_FALSE(users.empty());
    EXPECT_TRUE(appts.empty());
}