#include <gtest/gtest.h>
#include "system.h"

/* ================= TEST ACCESS ================= */

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

static int getFirstDoctorId(HospitalSystem &sys)
{
    return getDoctorId(sys);
}

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("DocName", "d@mail.com", "123", "cardio");
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("PatName", "p@mail.com", "123", "010");
}

static int bookOne(HospitalSystem &sys, int docId)
{
    sys.bookAppointment(docId, "2026", "10AM");
    return sys.viewMyAppointments().back().get_AppointmentId();
}

/* ================= LOGIN ================= */

TEST(SysFull, Login_NoMatchReturnsFalse)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("x", "y"));
}

TEST(SysFull, Login_EmptyCredentials)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("", ""));
}

TEST(SysFull, Login_AdminSuccess)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "admin");
}

TEST(SysFull, Login_PatientSuccess)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(SysFull, Login_DoctorSuccess)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_TRUE(sys.login("d@mail.com", "123"));
}

TEST(SysFull, Login_FailedDoesNotChangeUser)
{
    HospitalSystem sys;
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    auto *first = sys.getCurrentUser();

    sys.login("bad", "bad");

    EXPECT_EQ(sys.getCurrentUser(), first);
}

/* ================= BOOK ================= */

TEST(SysFull, Book_NoUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, Book_NotPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, Book_DoctorNotFound)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

TEST(SysFull, Book_Success)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(SysFull, Cancel_NoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

TEST(SysFull, Cancel_NotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

TEST(SysFull, Cancel_WrongPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    sys.login("p2@mail.com", "456");
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysFull, Cancel_Success)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

/* FORCE status != Scheduled branch */
TEST(SysFull, Cancel_AlreadyCompleted)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    sys.login("d@mail.com", "123");
    sys.completeAppointmentDoctor(id);

    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE ================= */

TEST(SysFull, Complete_NoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_NotDoctor)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_NotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

TEST(SysFull, Complete_Success)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* FORCE status != Scheduled */
TEST(SysFull, Complete_AfterCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    sys.cancelAppointmentPatient(id);

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= VIEW ================= */

TEST(SysFull, ViewMine_Null)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewMine_OK)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(getDoctorId(sys), "2026", "10AM");

    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}

TEST(SysFull, ViewDoctor_OK)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(getDoctorId(sys), "2026", "10AM");

    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
}

/* ================= ADMIN ================= */

TEST(SysFull, AdminUsers)
{
    SysTest sys;
    EXPECT_EQ(sys.adminViewAllUsers().size(), 1u);
}

TEST(SysFull, AdminAppointments)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    sys.bookAppointment(getDoctorId(sys), "2026", "10AM");

    EXPECT_EQ(sys.adminViewAllAppointments().size(), 1u);
}

/* ================= FIND DOCTOR COVERAGE ================= */

TEST(SysFull, FindDoctor_SkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

/* ================= CHAOS ================= */

TEST(SysFull, ChaosFlow)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int id = bookOne(sys, getDoctorId(sys));

    sys.cancelAppointmentPatient(id);
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}
