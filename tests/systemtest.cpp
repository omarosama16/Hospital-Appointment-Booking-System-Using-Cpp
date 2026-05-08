```cpp id="3jlwmk"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "system.h"

/* ================= TEST ACCESS ================= */

class SysTest : public HospitalSystem
{
};

/* ================= HELPERS ================= */

static int getDoctorId(HospitalSystem &sys)
{
    auto users = sys.adminViewAllUsers();

    for (auto *u : users)
    {
        if (u != nullptr && u->get_role() == "doctor")
        {
            return u->get_id();
        }
    }

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

/* ================= LOGIN ================= */

TEST(SysFull, LoginStress)
{
    HospitalSystem sys;

    setupPatient(sys);

    EXPECT_FALSE(sys.login("wrong", "wrong"));
    EXPECT_FALSE(sys.login("", ""));
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(SysFull, LoginInvalidCases)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("none@mail.com", "123"));
    EXPECT_FALSE(sys.login("", "123"));
    EXPECT_FALSE(sys.login("mail@test.com", ""));
}

/* ================= ROLE ================= */

TEST(SysFull, RoleSwitching)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    ASSERT_NE(sys.getCurrentUser(), nullptr);
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");

    sys.login("d@mail.com", "123");

    ASSERT_NE(sys.getCurrentUser(), nullptr);
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

/* ================= BOOKING ================= */

TEST(SysFull, BookingStress)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    ASSERT_NE(docId, -1);

    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    }

    EXPECT_FALSE(sys.bookAppointment(-1, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

TEST(SysFull, BookAppointmentNoUser)
{
    HospitalSystem sys;

    setupDoctor(sys);

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, BookAppointmentAsDoctor)
{
    HospitalSystem sys;

    setupDoctor(sys);

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, BookingInvalidDoctorIds)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(-1, "", ""));
    EXPECT_FALSE(sys.bookAppointment(0, "bad", "bad"));
    EXPECT_FALSE(sys.bookAppointment(99999, "2026", "10AM"));
}

/* ================= CANCEL ================= */

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

    ASSERT_FALSE(appts.empty());

    int id = appts[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
}

TEST(SysFull, CancelNoUser)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

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

    auto appts = sys.viewMyAppointments();

    ASSERT_FALSE(appts.empty());

    int id = appts[0].get_AppointmentId();

    sys.login("p2@mail.com", "456");

    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

/* ================= COMPLETE ================= */

TEST(SysFull, CompleteStress)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    ASSERT_NE(docId, -1);

    sys.bookAppointment(docId, "2026", "10AM");

    auto appts = sys.viewMyAppointments();

    ASSERT_FALSE(appts.empty());

    int id = appts[0].get_AppointmentId();

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
}

TEST(SysFull, CompleteNoUser)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, CompleteAsPatient)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

/* ================= VIEW ================= */

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

TEST(SysFull, ViewMyAppointmentsNoUser)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewDoctorScheduleNoUser)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= ADMIN ================= */

TEST(SysFull, AdminUsers)
{
    SysTest sys;

    auto users = sys.adminViewAllUsers();

    EXPECT_FALSE(users.empty());
}

TEST(SysFull, AdminAppointmentsEmpty)
{
    SysTest sys;

    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}

/* ================= STATE ================= */

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

/* ================= INVALID INPUT ================= */

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

    auto appts = sys.viewMyAppointments();

    ASSERT_FALSE(appts.empty());

    int id = appts[0].get_AppointmentId();

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
}

/* ================= EXTRA COVERAGE ================= */

TEST(SysFull, GetDoctorIdWithoutDoctor)
{
    HospitalSystem sys;

    EXPECT_EQ(getDoctorId(sys), -1);
}

TEST(SysFull, MultipleDoctors)
{
    HospitalSystem sys;

    setupDoctor(sys);

    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");

    EXPECT_NE(getDoctorId(sys), -1);
}

TEST(SysFull, MultiplePatients)
{
    HospitalSystem sys;

    setupPatient(sys);

    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");

    EXPECT_TRUE(sys.login("p2@mail.com", "456"));
}
```
