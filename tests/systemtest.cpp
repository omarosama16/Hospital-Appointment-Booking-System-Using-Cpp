#include <gtest/gtest.h>
#include "system.h"

/* ================= SETUP ================= */

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
}

static int getDoctorId(HospitalSystem &sys)
{
    auto users = sys.adminViewAllUsers();

    for (auto *u : users)
        if (u && u->get_role() == "doctor")
            return u->get_id();

    return -1;
}

/* ================= LOGIN CONDITIONS ================= */

TEST(SysOpt, LoginTrue)
{
    HospitalSystem sys;
    setupPatient(sys);

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(SysOpt, LoginFalse)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("wrong", "wrong"));
}

/* ================= BOOKING CONDITIONS ================= */

TEST(SysOpt, BookNoUser)
{
    HospitalSystem sys;

    setupDoctor(sys);

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysOpt, BookWrongRole)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("d@mail.com", "123"); // doctor

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(SysOpt, BookNoDoctorFound)
{
    HospitalSystem sys;

    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

TEST(SysOpt, BookSuccess)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= CANCEL CONDITIONS ================= */

TEST(SysOpt, CancelNoUser)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

TEST(SysOpt, CancelWrongPatient)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    sys.login("x", "y"); // force invalid state

    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysOpt, CancelNotScheduled)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    sys.cancelAppointmentPatient(id);

    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE CONDITIONS ================= */

TEST(SysOpt, CompleteNoUserOrNotDoctor)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysOpt, CompleteWrongDoctor)
{
    HospitalSystem sys;

    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    sys.login("d2@mail.com", "456");

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysOpt, CompleteSuccess)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* ================= VIEW CONDITIONS ================= */

TEST(SysOpt, ViewNoUser)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
