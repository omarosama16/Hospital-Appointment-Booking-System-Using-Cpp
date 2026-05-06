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

TEST(HospitalSystemTest, Login)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
}

TEST(HospitalSystemTest, BookingFlow)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026-01-01", "10AM"));

    auto appts = sys.viewMyAppointments();
    EXPECT_FALSE(appts.empty());

    int apptId = appts[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(apptId));
}

TEST(HospitalSystemTest, DoctorCompletion)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
}