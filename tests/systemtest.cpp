#include <gtest/gtest.h>
#include "system.h"

class SysTest : public HospitalSystem
{
};

static int getDoctorId(HospitalSystem &sys)
{
    auto users = sys.adminViewAllUsers();
    for (auto u : users)
        if (u && u->get_role() == "doctor")
            return u->get_id();
    return -1;
}

TEST(SysBoost, NoLogin_BlockAll)
{
    SysTest sys;

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));

    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(SysBoost, WrongRole_AdminCannotBook)
{
    SysTest sys;

    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysBoost, DoctorNotFound)
{
    SysTest sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");
    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

TEST(SysBoost, CancelThenCompleteBlocked)
{
    SysTest sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.cancelAppointmentPatient(id);

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysBoost, DoubleCompleteBlocked)
{
    SysTest sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    sys.bookAppointment(docId, "2026", "10AM");

    int id = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysBoost, CancelInvalidId)
{
    SysTest sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");
    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

TEST(SysBoost, CompleteInvalidId)
{
    SysTest sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}