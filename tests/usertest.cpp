#include <gtest/gtest.h>
#include "user.h"
#include "system.h"

/* ================= DEFAULT CONSTRUCTOR ================= */

TEST(UserTest, DefaultConstructor)
{
    User u;
    EXPECT_EQ(u.get_id(), 0);
    EXPECT_EQ(u.get_name(), "");
    EXPECT_EQ(u.get_email(), "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(), "");
}

/* ================= PARAMETERIZED CONSTRUCTOR ================= */

TEST(UserTest, ConstructorAndGetters)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_EQ(u.get_id(), 1);
    EXPECT_EQ(u.get_name(), "Omar");
    EXPECT_EQ(u.get_email(), "omar@test.com");
    EXPECT_EQ(u.get_password(), "1234");
    EXPECT_EQ(u.get_role(), "patient");
}

/* ================= SETTERS ================= */

TEST(UserTest, Setters)
{
    User u;
    u.set_id(99);
    u.set_name("Ali");
    u.set_email("ali@test.com");
    u.set_password("pass");
    u.set_role("doctor");
    EXPECT_EQ(u.get_id(), 99);
    EXPECT_EQ(u.get_name(), "Ali");
    EXPECT_EQ(u.get_email(), "ali@test.com");
    EXPECT_EQ(u.get_password(), "pass");
    EXPECT_EQ(u.get_role(), "doctor");
}

/* ================= AUTHENTICATE ================= */

// covers: email == e (true) && password == p (true)
TEST(UserTest, AuthenticateSuccess)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_TRUE(u.Authenticate("omar@test.com", "1234"));
}

// covers: email == e (true) && password == p (false)
TEST(UserTest, AuthenticateWrongPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", "wrong"));
}

// covers: email == e (false) — short-circuit, password never checked
TEST(UserTest, AuthenticateWrongEmail)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("wrong@test.com", "1234"));
}

// covers: both wrong
TEST(UserTest, AuthenticateBothWrong)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("wrong", "wrong"));
}

// covers: default constructed user authenticates with empty strings
TEST(UserTest, AuthenticateEmpty)
{
    User u;
    EXPECT_TRUE(u.Authenticate("", ""));
}

// FIX: covers email matches but password is empty — false branch of password check
TEST(UserTest, AuthenticateCorrectEmailEmptyPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("omar@test.com", ""));
}

// FIX: covers empty email with correct password — false branch of email check
TEST(UserTest, AuthenticateEmptyEmailCorrectPassword)
{
    User u(1, "Omar", "omar@test.com", "1234", "patient");
    EXPECT_FALSE(u.Authenticate("", "1234"));
}

/* ================= SYSTEM BOOST ================= */

TEST(SysBoost, NoLoginBlockAllActions)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(SysBoost, DoctorNotFoundBranch)
{
    HospitalSystem sys;
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(99999, "2026", "10AM"));
}

TEST(SysBoost, CancelThenCompleteEdge)
{
    HospitalSystem sys;
    sys.registerNewDoctor("d", "d@mail.com", "123", "cardio");
    sys.registerNewPatient("p", "p@mail.com", "123", "010");
    sys.login("p@mail.com", "123");
    int docId = sys.adminViewAllUsers()[0]->get_id() + 1;
    sys.bookAppointment(docId, "2026", "10AM");
    int id = sys.viewMyAppointments()[0].get_AppointmentId();
    sys.cancelAppointmentPatient(id);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}
