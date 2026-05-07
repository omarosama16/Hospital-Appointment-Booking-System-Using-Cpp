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

/* ================= LOGIN TESTS ================= */

TEST(Sys, Login_Success)
{
    HospitalSystem sys;
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(Sys, Login_Fail_Wrong)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("wrong", "wrong"));
}

TEST(Sys, Login_Fail_Empty)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("", ""));
}

/* ================= NULL USER PROTECTION ================= */

TEST(Sys, NullUser_Booking)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(Sys, NullUser_Cancel)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

TEST(Sys, NullUser_Complete)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(Sys, NullUser_View)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= BOOKING ================= */

TEST(Sys, Booking_Success)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(Sys, Booking_Fail_InvalidDoctor)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(9999, "2026", "10AM"));
}

TEST(Sys, Booking_Fail_WrongRole)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= CANCEL ================= */

TEST(Sys, Cancel_Success)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
}

TEST(Sys, Cancel_Fail_Invalid)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

/* ================= COMPLETE ================= */

TEST(Sys, Complete_Success)
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

TEST(Sys, Complete_Fail_Invalid)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

/* ================= VIEW ================= */

TEST(Sys, View_EmptyPatient)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(Sys, View_EmptyDoctor)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(Sys, View_NoLogin)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

/* ================= ROLE MISMATCH ================= */

TEST(Sys, Role_Block_Booking)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(Sys, Role_Block_Modify)
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

/* ================= FULL FLOW ================= */

TEST(Sys, Full_Flow)
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

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

/* ================= EXTRA STRESS ================= */

TEST(Sys, Stress_MultipleBookings)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    }

    EXPECT_FALSE(sys.viewMyAppointments().empty());
}