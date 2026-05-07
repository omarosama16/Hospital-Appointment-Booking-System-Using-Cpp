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
    {
        if (u && u->get_role() == "doctor")
            return u->get_id();
    }

    return -1;
}

TEST(HospitalSystemTest, LoginSuccess)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_TRUE(sys.login("p@mail.com", "123"));
}

TEST(HospitalSystemTest, LoginWrongPassword)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
}

TEST(HospitalSystemTest, LoginWrongEmail)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.login("wrong@mail.com", "123"));
}

TEST(HospitalSystemTest, BookAppointmentSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    EXPECT_TRUE(sys.bookAppointment(docId, "2026-01-01", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentWithoutLogin)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentAsAdmin)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("admin@mail.com", "admin123");

    int docId = getDoctorId(sys);

    EXPECT_FALSE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(HospitalSystemTest, BookAppointmentDoctorNotFound)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.bookAppointment(999, "2026", "10AM"));
}

TEST(HospitalSystemTest, CancelAppointmentSuccess)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);

    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_TRUE(sys.cancelAppointmentPatient(apptId));
}

TEST(HospitalSystemTest, CancelAppointmentInvalidId)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}

TEST(HospitalSystemTest, CancelWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

TEST(HospitalSystemTest, ViewAppointmentsEmpty)
{
    HospitalSystem sys;

    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    sys.login("p@mail.com", "123");

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(HospitalSystemTest, ViewAppointmentsWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(HospitalSystemTest, DoctorCompleteSuccess)
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

TEST(HospitalSystemTest, DoctorCompleteInvalidAppointment)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}

TEST(HospitalSystemTest, CompleteWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(HospitalSystemTest, ViewDoctorSchedule)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    sys.bookAppointment(docId, "2026", "10AM");

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.viewDoctorSchedule().empty());
}

TEST(HospitalSystemTest, ViewDoctorScheduleWithoutLogin)
{
    HospitalSystem sys;

    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(HospitalSystemTest, DoctorCannotCancelPatientAppointment)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    sys.login("d@mail.com", "123");

    EXPECT_FALSE(sys.cancelAppointmentPatient(apptId));
}

TEST(HospitalSystemTest, PatientCannotCompleteAppointment)
{
    HospitalSystem sys;

    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");

    int docId = getDoctorId(sys);

    sys.login("p@mail.com", "123");

    sys.bookAppointment(docId, "2026", "10AM");

    int apptId = sys.viewMyAppointments()[0].get_AppointmentId();

    EXPECT_FALSE(sys.completeAppointmentDoctor(apptId));
}

TEST(HospitalSystemTest, AdminViewUsers)
{
    TestSystem sys;

    auto users = sys.adminViewAllUsers();

    EXPECT_FALSE(users.empty());
}

TEST(HospitalSystemTest, AdminViewAppointments)
{
    TestSystem sys;

    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}