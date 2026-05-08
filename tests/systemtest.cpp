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

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("Doc", "d@mail.com", "123", "cardio");
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("Pat", "p@mail.com", "123", "010");
}

static int bookOne(HospitalSystem &sys, int docId)
{
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));

    auto appts = sys.viewMyAppointments();
    EXPECT_FALSE(appts.empty());

    return appts.back().get_AppointmentId();
}

/* ================= LOGIN ================= */

TEST(SysFull, Login_Fail)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("x", "y"));
}

TEST(SysFull, Login_SuccessAdmin)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "admin");
}

/* ================= BOOKING ================= */

TEST(SysFull, Book_NoUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, Book_WrongRole)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
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

TEST(SysFull, Cancel_WrongPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.registerNewPatient("P2", "p2@mail.com", "456", "011");

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id = bookOne(sys, docId);

    sys.login("p2@mail.com", "456");

    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysFull, Cancel_Success)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id = bookOne(sys, docId);

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE ================= */

TEST(SysFull, Complete_WrongRole)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_SuccessFlow)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id = bookOne(sys, docId);

    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* ================= VIEW ================= */

TEST(SysFull, ViewEmpty)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewAfterBooking)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    bookOne(sys, docId);

    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}

/* ================= EDGE COVERAGE ================= */

TEST(SysFull, FindDoctorNotFound)
{
    HospitalSystem sys;
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

TEST(SysFull, CancelAlreadyCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id = bookOne(sys, docId);

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysFull, CompleteAfterCancel)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id = bookOne(sys, docId);

    sys.cancelAppointmentPatient(id);

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= STRESS ================= */

TEST(SysFull, Chaos)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.login("bad", "bad"));

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);

    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);

    sys.cancelAppointmentPatient(id1);

    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id2));
}
