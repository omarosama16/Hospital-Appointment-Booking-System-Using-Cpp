#include <gtest/gtest.h>
#include "system.h"
#include "user.h"
#include "Appointment.h"
#include "Doctor.h"
#include "Patient.h"
#include "admin.h"
#include <sstream>
#include <set>

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

static int getFirstDoctorId(HospitalSystem &sys)
{
    for (auto *u : sys.adminViewAllUsers())
        if (u && u->get_role() == "doctor")
            return u->get_id();
    return -1;
}

static int getPatientId(HospitalSystem &sys, const std::string &email)
{
    for (auto *u : sys.adminViewAllUsers())
        if (u && u->get_role() == "patient" && u->get_email() == email)
            return u->get_id();
    return -1;
}

static void setupDoctor(HospitalSystem &sys)
{
    sys.registerNewDoctor("DocName", "d@mail.com", "123", "cardio");
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("PatName", "p@mail.com", "123", "010");
}

static int bookOne(HospitalSystem &sys, int docId)
{
    sys.bookAppointment(docId, "2026", "10AM");
    return sys.viewMyAppointments().back().get_AppointmentId();
}

/* ============================================================ */
/* ================= DIRECT CLASS TESTS ======================= */
/* ============================================================ */

/* ================= USER CLASS ================= */

TEST(UserDirect, DefaultConstructor)
{
    User u;
    EXPECT_EQ(u.get_id(), 0);
    EXPECT_EQ(u.get_name(), "");
    EXPECT_EQ(u.get_email(), "");
    EXPECT_EQ(u.get_password(), "");
    EXPECT_EQ(u.get_role(), "");
}

TEST(UserDirect, ParameterizedConstructor)
{
    User u(1, "TestName", "test@mail.com", "pass123", "admin");
    EXPECT_EQ(u.get_id(), 1);
    EXPECT_EQ(u.get_name(), "TestName");
    EXPECT_EQ(u.get_email(), "test@mail.com");
    EXPECT_EQ(u.get_password(), "pass123");
    EXPECT_EQ(u.get_role(), "admin");
}

TEST(UserDirect, Setters)
{
    User u;
    u.set_id(42);
    u.set_name("NewName");
    u.set_email("new@mail.com");
    u.set_password("newpass");
    u.set_role("doctor");

    EXPECT_EQ(u.get_id(), 42);
    EXPECT_EQ(u.get_name(), "NewName");
    EXPECT_EQ(u.get_email(), "new@mail.com");
    EXPECT_EQ(u.get_password(), "newpass");
    EXPECT_EQ(u.get_role(), "doctor");
}

TEST(UserDirect, Authenticate_BothCorrect)
{
    User u(1, "Name", "email@mail.com", "password", "role");
    EXPECT_TRUE(u.Authenticate("email@mail.com", "password"));
}

TEST(UserDirect, Authenticate_CorrectEmailWrongPassword)
{
    User u(1, "Name", "email@mail.com", "password", "role");
    EXPECT_FALSE(u.Authenticate("email@mail.com", "wrong"));
}

TEST(UserDirect, Authenticate_WrongEmailCorrectPassword)
{
    User u(1, "Name", "email@mail.com", "password", "role");
    EXPECT_FALSE(u.Authenticate("wrong@mail.com", "password"));
}

TEST(UserDirect, Authenticate_BothWrong)
{
    User u(1, "Name", "email@mail.com", "password", "role");
    EXPECT_FALSE(u.Authenticate("wrong@mail.com", "wrong"));
}

TEST(UserDirect, Authenticate_EmptyEmail)
{
    User u(1, "Name", "", "password", "role");
    EXPECT_TRUE(u.Authenticate("", "password"));
}

TEST(UserDirect, Authenticate_EmptyPassword)
{
    User u(1, "Name", "email@mail.com", "", "role");
    EXPECT_TRUE(u.Authenticate("email@mail.com", ""));
}

TEST(UserDirect, Authenticate_EmptyBoth)
{
    User u(1, "Name", "", "", "role");
    EXPECT_TRUE(u.Authenticate("", ""));
}

TEST(UserDirect, Authenticate_CaseSensitive)
{
    User u(1, "Name", "Email@Mail.COM", "Pass", "role");
    EXPECT_FALSE(u.Authenticate("email@mail.com", "Pass"));
}

/* ================= ADMIN CLASS ================= */

TEST(AdminDirect, Constructor)
{
    Admin a(1, "AdminName", "admin@mail.com", "adminpass");
    EXPECT_EQ(a.get_id(), 1);
    EXPECT_EQ(a.get_name(), "AdminName");
    EXPECT_EQ(a.get_email(), "admin@mail.com");
    EXPECT_EQ(a.get_password(), "adminpass");
    EXPECT_EQ(a.get_role(), "admin");
}

/* ================= DOCTOR CLASS ================= */

TEST(DoctorDirect, Constructor)
{
    Doctor d(1, "Doc", "doc@mail.com", "pass", "cardio");
    EXPECT_EQ(d.get_id(), 1);
    EXPECT_EQ(d.get_name(), "Doc");
    EXPECT_EQ(d.get_email(), "doc@mail.com");
    EXPECT_EQ(d.get_password(), "pass");
    EXPECT_EQ(d.get_role(), "doctor");
    EXPECT_EQ(d.getSpecialization(), "cardio");
}

TEST(DoctorDirect, Availability)
{
    Doctor d(1, "Doc", "doc@mail.com", "pass", "cardio");

    EXPECT_TRUE(d.getAvailability().empty());

    d.addAvailability("10AM");
    d.addAvailability("11AM");
    d.addAvailability("02PM");

    auto avail = d.getAvailability();
    EXPECT_EQ(avail.size(), 3u);
    EXPECT_EQ(avail[0], "10AM");
    EXPECT_EQ(avail[1], "11AM");
    EXPECT_EQ(avail[2], "02PM");

    d.clearAvailability();
    EXPECT_TRUE(d.getAvailability().empty());
}

/* ================= PATIENT CLASS ================= */

TEST(PatientDirect, Constructor)
{
    Patient p(1, "Pat", "pat@mail.com", "pass", "010");
    EXPECT_EQ(p.get_id(), 1);
    EXPECT_EQ(p.get_name(), "Pat");
    EXPECT_EQ(p.get_email(), "pat@mail.com");
    EXPECT_EQ(p.get_password(), "pass");
    EXPECT_EQ(p.get_role(), "patient");
}

/* ================= APPOINTMENT CLASS ================= */

TEST(AppointmentDirect, DefaultConstructor)
{
    Appointment a;
    EXPECT_EQ(a.get_AppointmentId(), 0);
    EXPECT_EQ(a.get_PatientId(), 0);
    EXPECT_EQ(a.get_DoctorId(), 0);
    EXPECT_EQ(a.get_Date(), "");
    EXPECT_EQ(a.get_Time(), "");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

TEST(AppointmentDirect, ParameterizedConstructor)
{
    AppointmentInfo info{"PatName", "DocName", "2026-01-01", "10AM"};
    Appointment a(1, 2, 3, info, Status::Scheduled);

    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_PatientId(), 2);
    EXPECT_EQ(a.get_DoctorId(), 3);
    EXPECT_EQ(a.get_Date(), "2026-01-01");
    EXPECT_EQ(a.get_Time(), "10AM");
    EXPECT_EQ(a.get_Status(), Status::Scheduled);
}

TEST(AppointmentDirect, Cancel_FromScheduled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Scheduled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentDirect, Cancel_FromCancelled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Cancelled);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentDirect, Cancel_FromCompleted)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Completed);
    a.cancel();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentDirect, Complete_FromScheduled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Scheduled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentDirect, Complete_FromCompleted)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Completed);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Completed);
}

TEST(AppointmentDirect, Complete_FromCancelled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Cancelled);
    a.complete();
    EXPECT_EQ(a.get_Status(), Status::Cancelled);
}

TEST(AppointmentDirect, PrintRow_Scheduled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Scheduled);

    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    a.print_row();
    std::cout.rdbuf(old);

    std::string output = oss.str();
    EXPECT_NE(output.find("Scheduled"), std::string::npos);
    EXPECT_NE(output.find("Pat"), std::string::npos);
    EXPECT_NE(output.find("Doc"), std::string::npos);
}

TEST(AppointmentDirect, PrintRow_Cancelled)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Cancelled);

    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    a.print_row();
    std::cout.rdbuf(old);

    std::string output = oss.str();
    EXPECT_NE(output.find("Cancelled"), std::string::npos);
}

TEST(AppointmentDirect, PrintRow_Completed)
{
    AppointmentInfo info{"Pat", "Doc", "2026", "10AM"};
    Appointment a(1, 2, 3, info, Status::Completed);

    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    a.print_row();
    std::cout.rdbuf(old);

    std::string output = oss.str();
    EXPECT_NE(output.find("Completed"), std::string::npos);
}

/* ============================================================ */
/* ================= SYSTEM TESTS ============================== */
/* ============================================================ */

/* ================= CONSTRUCTOR & BASIC STATE ================= */

TEST(SysFull, Constructor_InitialState)
{
    HospitalSystem sys;
    EXPECT_EQ(sys.getCurrentUser(), nullptr);

    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 1u);
    EXPECT_EQ(users[0]->get_role(), "admin");
    EXPECT_EQ(users[0]->get_name(), "admin");
    EXPECT_EQ(users[0]->get_email(), "admin@mail.com");
}

TEST(SysFull, GetCurrentUser_InitiallyNull)
{
    HospitalSystem sys;
    EXPECT_EQ(sys.getCurrentUser(), nullptr);
}

TEST(SysFull, GetCurrentUser_StablePointer)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");

    User *u1 = sys.getCurrentUser();
    User *u2 = sys.getCurrentUser();
    EXPECT_EQ(u1, u2);
    EXPECT_NE(u1, nullptr);
}

/* ================= LOGIN ================= */

TEST(SysFull, Login_NoMatchReturnsfalse)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("nobody@mail.com", "wrong"));
}

TEST(SysFull, Login_EmptyCredentials)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("", ""));
}

TEST(SysFull, Login_WhitespaceOnly)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("   ", "   "));
}

TEST(SysFull, Login_AdminSuccess)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "admin");
}

TEST(SysFull, Login_PatientSuccess)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");
}

TEST(SysFull, Login_DoctorSuccess)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_TRUE(sys.login("d@mail.com", "123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

TEST(SysFull, Login_RightEmailWrongPassword)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
}

TEST(SysFull, Login_WrongEmailRightPassword)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("wrong@mail.com", "123"));
}

TEST(SysFull, Login_FailedDoesNotClearCurrentUser)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_NE(sys.getCurrentUser(), nullptr);
    sys.login("bad", "bad");
    EXPECT_NE(sys.getCurrentUser(), nullptr);
}

TEST(SysFull, Login_RoleSwitching)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");
    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

TEST(SysFull, Login_CaseSensitiveEmail)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("P@MAIL.COM", "123"));
}

TEST(SysFull, Login_MultipleFailures)
{
    HospitalSystem sys;
    for (int i = 0; i < 100; i++)
        EXPECT_FALSE(sys.login("bad", "bad"));
}

/* ================= REGISTRATION ================= */

TEST(SysFull, RegisterPatient_EmptyPhone)
{
    HospitalSystem sys;
    sys.registerNewPatient("PatEmpty", "pempty@mail.com", "pass", "");
    EXPECT_TRUE(sys.login("pempty@mail.com", "pass"));
}

TEST(SysFull, RegisterPatient_IdIncrement)
{
    HospitalSystem sys;
    sys.registerNewPatient("Pat1", "p1@mail.com", "123", "010");
    sys.registerNewPatient("Pat2", "p2@mail.com", "456", "011");

    auto users = sys.adminViewAllUsers();
    int id1 = -1, id2 = -1;
    for (auto *u : users) {
        if (u->get_email() == "p1@mail.com") id1 = u->get_id();
        if (u->get_email() == "p2@mail.com") id2 = u->get_id();
    }
    EXPECT_NE(id1, -1);
    EXPECT_NE(id2, -1);
    EXPECT_EQ(id2, id1 + 1);
}

TEST(SysFull, RegisterDoctor_EmptySpecialty)
{
    HospitalSystem sys;
    sys.registerNewDoctor("DocEmpty", "dempty@mail.com", "pass", "");
    EXPECT_TRUE(sys.login("dempty@mail.com", "pass"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

TEST(SysFull, RegisterDoctor_AvailabilityAdded)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_TRUE(sys.login("d@mail.com", "123"));
}

TEST(SysFull, RegisterDoctor_MultipleSpecialties)
{
    HospitalSystem sys;
    sys.registerNewDoctor("D1", "d1@mail.com", "p1", "cardio");
    sys.registerNewDoctor("D2", "d2@mail.com", "p2", "neuro");
    sys.registerNewDoctor("D3", "d3@mail.com", "p3", "ortho");

    auto users = sys.adminViewAllUsers();
    int docCount = 0;
    for (auto *u : users)
        if (u->get_role() == "doctor")
            docCount++;
    EXPECT_EQ(docCount, 3);
}

/* ================= BOOK APPOINTMENT ================= */

TEST(SysFull, Book_NoCurrentUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

TEST(SysFull, Book_AsDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

TEST(SysFull, Book_AsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

TEST(SysFull, Book_DoctorNotFound)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(99999, "2026", "10AM"));
}

TEST(SysFull, Book_NegativeDoctorId)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(-1, "2026", "10AM"));
}

TEST(SysFull, Book_DoctorIdZero)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(0, "2026", "10AM"));
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
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}

TEST(SysFull, Book_MultipleAppointments)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);

    std::vector<int> ids;
    for (int i = 0; i < 3; i++) {
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
        ids.push_back(sys.viewMyAppointments().back().get_AppointmentId());
    }
    EXPECT_EQ(sys.viewMyAppointments().size(), 3u);
    EXPECT_LT(ids[0], ids[1]);
    EXPECT_LT(ids[1], ids[2]);
}

TEST(SysFull, Book_EmptyDate)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "", "10AM"));
}

TEST(SysFull, Book_EmptyTime)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", ""));
}

TEST(SysFull, Book_EmptyDateAndTime)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "", ""));
}

TEST(SysFull, Book_FindDoctorSkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2","p2@mail.com","456","011");
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

TEST(SysFull, Book_DifferentTimes)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026-01-01", "09:00"));
    EXPECT_TRUE(sys.bookAppointment(docId, "2026-01-02", "14:30"));
    EXPECT_TRUE(sys.bookAppointment(docId, "2026-12-31", "23:59"));
    EXPECT_EQ(sys.viewMyAppointments().size(), 3u);
}

/* ================= CANCEL APPOINTMENT ================= */

TEST(SysFull, Cancel_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

TEST(SysFull, Cancel_IdNotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(99999));
}

TEST(SysFull, Cancel_NegativeId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
}

TEST(SysFull, Cancel_WrongPatient)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
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
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    sys.login("p@mail.com", "123");
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);
}

TEST(SysFull, Cancel_AlreadyCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysFull, Cancel_AfterCompleted)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

TEST(SysFull, Cancel_SecondAppointment)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id2));
    EXPECT_TRUE(sys.cancelAppointmentPatient(id1));
}

TEST(SysFull, Cancel_FirstAppointmentSecondRemains)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id1));
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 2u);
    bool foundScheduled = false;
    for (const auto &a : appts) {
        if (a.get_AppointmentId() == id2 && a.get_Status() == Status::Scheduled)
            foundScheduled = true;
    }
    EXPECT_TRUE(foundScheduled);
}

/* ================= COMPLETE APPOINTMENT ================= */

TEST(SysFull, Complete_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_AsPatient)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_AsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

TEST(SysFull, Complete_IdNotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(99999));
}

TEST(SysFull, Complete_NegativeId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
}

TEST(SysFull, Complete_WrongDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getFirstDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d2@mail.com", "456");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysFull, Complete_Success)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    auto schedule = sys.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1u);
    EXPECT_EQ(schedule[0].get_Status(), Status::Completed);
}

TEST(SysFull, Complete_AlreadyCompleted)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysFull, Complete_AfterCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

TEST(SysFull, Complete_SecondAppointment)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id2));
    EXPECT_TRUE(sys.completeAppointmentDoctor(id1));
}

TEST(SysFull, Complete_FirstAppointmentSecondRemains)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id1));
    auto schedule = sys.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 2u);
    bool foundCompleted = false;
    bool foundScheduled = false;
    for (const auto &a : schedule) {
        if (a.get_AppointmentId() == id1 && a.get_Status() == Status::Completed)
            foundCompleted = true;
        if (a.get_AppointmentId() == id2 && a.get_Status() == Status::Scheduled)
            foundScheduled = true;
    }
    EXPECT_TRUE(foundCompleted);
    EXPECT_TRUE(foundScheduled);
}

/* ================= VIEW MY APPOINTMENTS ================= */

TEST(SysFull, ViewMine_NoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewMine_EmptySchedule)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewMine_MatchingAppointments)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    sys.bookAppointment(docId, "2026", "11AM");
    EXPECT_EQ(sys.viewMyAppointments().size(), 2u);
}

TEST(SysFull, ViewMine_OtherPatientFiltered)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    sys.login("p2@mail.com", "456");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

TEST(SysFull, ViewMine_TwoPatientsIsolated)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");
    sys.login("p2@mail.com", "456");
    sys.bookAppointment(docId, "2026", "11AM");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}

TEST(SysFull, ViewMine_ShowsCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id);
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);
}

TEST(SysFull, ViewMine_ShowsCompleted)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    sys.completeAppointmentDoctor(id);
    sys.login("p@mail.com", "123");
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Completed);
}

/* ================= VIEW DOCTOR SCHEDULE ================= */

TEST(SysFull, ViewDoc_NoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(SysFull, ViewDoc_EmptySchedule)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(SysFull, ViewDoc_MatchingAppointments)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    sys.bookAppointment(docId, "2026", "11AM");
    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 2u);
}

TEST(SysFull, ViewDoc_OtherDoctorFiltered)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);
    int docId = getFirstDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.login("p@mail.com", "123");
    sys.bookAppointment(docId, "2026", "10AM");
    sys.login("d2@mail.com", "456");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

TEST(SysFull, ViewDoc_TwoDoctorsIsolated)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId1 = -1, docId2 = -1;
    for (auto *u : sys.adminViewAllUsers()) {
        if (u->get_role() == "doctor") {
            if (docId1 == -1) docId1 = u->get_id();
            else              docId2 = u->get_id();
        }
    }
    ASSERT_NE(docId1, -1);
    ASSERT_NE(docId2, -1);
    sys.bookAppointment(docId1, "2026", "10AM");
    sys.bookAppointment(docId2, "2026", "11AM");
    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
    sys.login("d2@mail.com", "456");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
}

TEST(SysFull, ViewDoc_ShowsCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id);
    sys.login("d@mail.com", "123");
    auto schedule = sys.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1u);
    EXPECT_EQ(schedule[0].get_Status(), Status::Cancelled);
}

TEST(SysFull, ViewDoc_ShowsCompleted)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    sys.completeAppointmentDoctor(id);
    auto schedule = sys.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1u);
    EXPECT_EQ(schedule[0].get_Status(), Status::Completed);
}

/* ================= ADMIN VIEWS ================= */

TEST(SysFull, AdminViewUsers_OnlyAdmin)
{
    HospitalSystem sys;
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 1u);
    EXPECT_EQ(users[0]->get_role(), "admin");
}

TEST(SysFull, AdminViewUsers_WithAll)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 3u);
}

TEST(SysFull, AdminViewUsers_VerifyProperties)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    auto users = sys.adminViewAllUsers();
    bool foundAdmin = false, foundDoctor = false, foundPatient = false;
    for (auto *u : users) {
        if (u->get_role() == "admin" && u->get_email() == "admin@mail.com")
            foundAdmin = true;
        if (u->get_role() == "doctor" && u->get_name() == "DocName")
            foundDoctor = true;
        if (u->get_role() == "patient" && u->get_name() == "PatName")
            foundPatient = true;
    }
    EXPECT_TRUE(foundAdmin);
    EXPECT_TRUE(foundDoctor);
    EXPECT_TRUE(foundPatient);
}

TEST(SysFull, AdminViewAppointments_Empty)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}

TEST(SysFull, AdminViewAppointments_AfterBooking)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 1u);
}

TEST(SysFull, AdminViewAppointments_AfterCancellation)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id);
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);
}

TEST(SysFull, AdminViewAppointments_AfterCompletion)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    sys.completeAppointmentDoctor(id);
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Completed);
}

TEST(SysFull, AdminViewAppointments_MixedStatuses)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    int id3 = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id1);
    sys.login("d@mail.com", "123");
    sys.completeAppointmentDoctor(id2);
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts.size(), 3u);
    int scheduledCount = 0, cancelledCount = 0, completedCount = 0;
    for (const auto &a : appts) {
        if (a.get_Status() == Status::Scheduled) scheduledCount++;
        if (a.get_Status() == Status::Cancelled) cancelledCount++;
        if (a.get_Status() == Status::Completed) completedCount++;
    }
    EXPECT_EQ(scheduledCount, 1);
    EXPECT_EQ(cancelledCount, 1);
    EXPECT_EQ(completedCount, 1);
}

/* ================= FIND DOCTOR ================= */

TEST(SysFull, FindDoctor_SkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

TEST(SysFull, FindDoctor_WrongDoctorId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(99, "2026", "10AM"));
}

TEST(SysFull, FindDoctor_IdZero)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(0, "2026", "10AM"));
}

TEST(SysFull, FindDoctor_SecondDoctor)
{
    HospitalSystem sys;
    sys.registerNewDoctor("d1", "d1@mail.com", "p1", "cardio");
    sys.registerNewDoctor("d2", "d2@mail.com", "p2", "neuro");
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId2 = -1;
    for (auto *u : sys.adminViewAllUsers()) {
        if (u->get_role() == "doctor" && u->get_name() == "d2")
            docId2 = u->get_id();
    }
    ASSERT_NE(docId2, -1);
    EXPECT_TRUE(sys.bookAppointment(docId2, "2026", "10AM"));
}

/* ================= ID MANAGEMENT ================= */

TEST(SysFull, AppointmentId_UniqueAndIncrementing)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    std::set<int> ids;
    for (int i = 0; i < 5; i++) {
        sys.bookAppointment(docId, "2026", "10AM");
        int id = sys.viewMyAppointments().back().get_AppointmentId();
        EXPECT_EQ(ids.count(id), 0u);
        ids.insert(id);
    }
}

TEST(SysFull, UserId_Unique)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");
    sys.registerNewDoctor("d2", "d2@mail.com", "789", "ortho");
    auto users = sys.adminViewAllUsers();
    std::set<int> ids;
    for (auto *u : users) {
        EXPECT_EQ(ids.count(u->get_id()), 0u);
        ids.insert(u->get_id());
    }
}

/* ================= CHAOS / STRESS ================= */

TEST(SysFull, ChaosFlow)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    EXPECT_FALSE(sys.login("x", "y"));
    EXPECT_FALSE(sys.login("", ""));
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id1 = bookOne(sys, docId);
    int id2 = bookOne(sys, docId);
    sys.cancelAppointmentPatient(id1);
    EXPECT_FALSE(sys.cancelAppointmentPatient(id1));
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id1));
    EXPECT_TRUE(sys.completeAppointmentDoctor(id2));
    EXPECT_FALSE(sys.completeAppointmentDoctor(id2));
}

TEST(SysFull, InvalidInputBomb)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(-1, "", ""));
    EXPECT_FALSE(sys.bookAppointment(99999, "bad", "bad"));
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
    EXPECT_FALSE(sys.cancelAppointmentPatient(99999));
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
    EXPECT_FALSE(sys.completeAppointmentDoctor(99999));
}

TEST(SysFull, Stress_ManyAppointments)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    const int N = 100;
    for (int i = 0; i < N; i++)
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    EXPECT_EQ(sys.viewMyAppointments().size(), static_cast<size_t>(N));
}

TEST(SysFull, Stress_ManyUsers)
{
    HospitalSystem sys;
    const int N = 50;
    for (int i = 0; i < N; i++) {
        sys.registerNewPatient("P" + std::to_string(i), 
                                "p" + std::to_string(i) + "@mail.com", 
                                "pass", "010");
    }
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), static_cast<size_t>(N + 1));
}

TEST(SysFull, Stress_RapidRoleSwitching)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    for (int i = 0; i < 50; i++) {
        EXPECT_TRUE(sys.login("p@mail.com", "123"));
        EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");
        EXPECT_TRUE(sys.login("d@mail.com", "123"));
        EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
    }
}

/* ================= INTEGRATION WORKFLOWS ================= */

TEST(SysFull, Workflow_BookCancelAdmin)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);
    auto allAppts = sys.adminViewAllAppointments();
    EXPECT_EQ(allAppts.size(), 1u);
    EXPECT_EQ(allAppts[0].get_Status(), Status::Cancelled);
}

TEST(SysFull, Workflow_BookCompleteVerify)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));
    auto docSchedule = sys.viewDoctorSchedule();
    EXPECT_EQ(docSchedule.size(), 1u);
    EXPECT_EQ(docSchedule[0].get_Status(), Status::Completed);
    sys.login("p@mail.com", "123");
    auto patAppts = sys.viewMyAppointments();
    EXPECT_EQ(patAppts.size(), 1u);
    EXPECT_EQ(patAppts[0].get_Status(), Status::Completed);
}

TEST(SysFull, Workflow_MultiplePatientsOneDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.registerNewPatient("p2", "p2@mail.com", "456", "011");
    sys.registerNewPatient("p3", "p3@mail.com", "789", "012");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.login("p@mail.com", "123");
    bookOne(sys, docId);
    sys.login("p2@mail.com", "456");
    bookOne(sys, docId);
    sys.login("p3@mail.com", "789");
    bookOne(sys, docId);
    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 3u);
    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
    sys.login("p2@mail.com", "456");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
    sys.login("p3@mail.com", "789");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}
