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

// FIX: covers bookAppointment with no current user
TEST(SysFull, BookAppointmentNoUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

// FIX: covers bookAppointment when logged in as doctor (not patient)
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

// FIX: covers cancelAppointmentPatient with no current user
TEST(SysFull, CancelNoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
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

// FIX: covers completeAppointmentDoctor with no current user
TEST(SysFull, CompleteNoUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
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

// FIX: covers viewMyAppointments with no current user
TEST(SysFull, ViewMyAppointmentsNoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// FIX: covers viewDoctorSchedule with no current user
TEST(SysFull, ViewDoctorScheduleNoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= ADMIN ================= */

TEST(SysFull, AdminStress)
{
    SysTest sys;
    EXPECT_FALSE(sys.adminViewAllUsers().empty());
}

// FIX: covers adminViewAllUsers with doctors and patients populated
TEST(SysFull, AdminViewAllUsersWithData)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);

    auto users = sys.adminViewAllUsers();
    EXPECT_GE(users.size(), 3u); // admin + doctor + patient
    EXPECT_NE(users[0], nullptr);
}

// FIX: covers adminViewAllAppointments with data
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

// FIX: covers adminViewAllAppointments when empty
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
