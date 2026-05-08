#include <gtest/gtest.h>
#include "system.h"

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

    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i] != nullptr &&
            users[i]->get_role() == "doctor")
        {
            return users[i]->get_id();
        }
    }

    return -1;
}

TEST(SysFull, LoginWorks)
{
    HospitalSystem sys;

    setupPatient(sys);

    EXPECT_TRUE(
        sys.login("p@mail.com", "123")
    );
}

TEST(SysFull, InvalidLogin)
{
    HospitalSystem sys;

    EXPECT_FALSE(
        sys.login("wrong", "wrong")
    );
}

TEST(SysFull, BookingWorks)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int doctorId = getDoctorId(sys);

    ASSERT_NE(doctorId, -1);

    EXPECT_TRUE(
        sys.bookAppointment(
            doctorId,
            "2026",
            "10AM"
        )
    );
}

TEST(SysFull, InvalidBooking)
{
    HospitalSystem sys;

    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(
        sys.bookAppointment(
            -1,
            "",
            ""
        )
    );
}

TEST(SysFull, CancelInvalid)
{
    HospitalSystem sys;

    EXPECT_FALSE(
        sys.cancelAppointmentPatient(-1)
    );
}

TEST(SysFull, CompleteInvalid)
{
    HospitalSystem sys;

    EXPECT_FALSE(
        sys.completeAppointmentDoctor(-1)
    );
}

TEST(SysFull, EmptyViews)
{
    HospitalSystem sys;

    EXPECT_TRUE(
        sys.viewMyAppointments().empty()
    );

    EXPECT_TRUE(
        sys.viewDoctorSchedule().empty()
    );
}
