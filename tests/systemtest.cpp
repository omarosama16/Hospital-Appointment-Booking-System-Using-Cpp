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
    for (auto u : sys.adminViewAllUsers())
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

/* ================= LOGIN EDGE ================= */

TEST(SysFull, LoginStress)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("wrong", "wrong"));
    EXPECT_FALSE(sys.login("", ""));
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

/* ================= ROLE SWITCHING ================= */

TEST(SysFull, RoleSwitching)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");

    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

/* ================= BOOKING STRESS ================= */

TEST(SysFull, BookingStress)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    for (int i = 0; i < 5; i++)
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));

    EXPECT_FALSE(sys.bookAppointment(-1, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

// covers bookAppointment with no current user
TEST(SysFull, BookAppointmentNoUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

// covers bookAppointment when logged in as doctor (not patient)
TEST(SysFull, BookAppointmentAsDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

/* ================= CANCEL EDGE ================= */

TEST(SysFull, CancelStress)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
}

// covers cancelAppointmentPatient with no current user
TEST(SysFull, CancelNoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

// FIX: appointment id matches but patient id does not — false branch of && in cancelAppointmentPatient
TEST(SysFull, CancelWrongPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    // different patient tries to cancel — id matches but patient id doesn't
    sys.login("p2@mail.com", "456");
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE EDGE ================= */

TEST(SysFull, CompleteStress)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");
    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
}

// covers completeAppointmentDoctor with no current user — false branch of || left side
TEST(SysFull, CompleteNoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// FIX: logged in as patient — covers false branch of || right side (role != "doctor")
TEST(SysFull, CompleteAsPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// FIX: appointment id matches but doctor id does not — false branch of && in completeAppointmentDoctor
TEST(SysFull, CompleteWrongDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    // different doctor tries to complete — id matches but doctor id doesn't
    sys.login("d2@mail.com", "456");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= VIEW EDGE ================= */

TEST(SysFull, ViewStress)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");
    EXPECT_FALSE(sys.viewMyAppointments().empty());

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.viewDoctorSchedule().empty());
}

// covers viewMyAppointments with no current user
TEST(SysFull, ViewMyAppointmentsNoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// covers viewDoctorSchedule with no current user
TEST(SysFull, ViewDoctorScheduleNoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

// FIX: appointments exist but belong to different patient — false branch of if in viewMyAppointments
TEST(SysFull, ViewMyAppointmentsOtherPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");

    // p2 logs in — loop runs but no appointments match
    sys.login("p2@mail.com", "456");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// FIX: appointments exist but belong to different doctor — false branch of if in viewDoctorSchedule
TEST(SysFull, ViewDoctorScheduleOtherDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");

    // d2 logs in — loop runs but no appointments match
    sys.login("d2@mail.com", "456");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= ADMIN ================= */

TEST(SysFull, AdminStress)
{
    SysTest sys;
    EXPECT_FALSE(sys.adminViewAllUsers().empty());
}

TEST(SysFull, AdminViewAllUsersWithData)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);

    auto users = sys.adminViewAllUsers();
    EXPECT_GE(users.size(), 3u);
    EXPECT_NE(users[0], nullptr);
}

TEST(SysFull, AdminViewAllAppointments)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");
    EXPECT_FALSE(sys.adminViewAllAppointments().empty());
}

TEST(SysFull, AdminViewAllAppointmentsEmpty)
{
    SysTest sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}

/* ================= STATE POISONING ================= */

TEST(SysFull, StatePoisoning)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.login("bad", "bad"));
    EXPECT_TRUE(sys.login("p@mail.com", "123"));

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    EXPECT_FALSE(sys.login("x", "y"));
    EXPECT_FALSE(sys.viewMyAppointments().empty());
}

/* ================= INVALID INPUT BOMB ================= */

TEST(SysFull, InvalidInputBomb)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(-1, "", ""));
    EXPECT_FALSE(sys.bookAppointment(99999, "bad", "bad"));
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
}

/* ================= ROLE ABUSE ================= */

TEST(SysFull, RoleAbuse)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");
    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));

    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* ================= CHAOS FLOW ================= */

TEST(SysFull, ChaosScenario)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    EXPECT_FALSE(sys.login("x", "y"));
    EXPECT_FALSE(sys.login("", ""));

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();
    int id = appts[0].get_AppointmentId();

    sys.cancelAppointmentPatient(id);
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}
