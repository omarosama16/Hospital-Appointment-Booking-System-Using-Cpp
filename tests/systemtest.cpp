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

/* ================= LOGIN ================= */

// login: find_if returns end() → false
TEST(SysFull, Login_NoMatchReturnsfalse)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("nobody@mail.com", "wrong"));
}

// login: empty credentials → false
TEST(SysFull, Login_EmptyCredentials)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("", ""));
}

// login: correct admin credentials → true
TEST(SysFull, Login_AdminSuccess)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "admin");
}

// login: correct patient credentials → true
TEST(SysFull, Login_PatientSuccess)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_TRUE(sys.login("p@mail.com", "123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "patient");
}

// login: correct doctor credentials → true
TEST(SysFull, Login_DoctorSuccess)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_TRUE(sys.login("d@mail.com", "123"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

// login: right email wrong password → false
TEST(SysFull, Login_RightEmailWrongPassword)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("p@mail.com", "wrong"));
}

// login: wrong email right password → false
TEST(SysFull, Login_WrongEmailRightPassword)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("wrong@mail.com", "123"));
}

// login: failed login does NOT clear currentUser
TEST(SysFull, Login_FailedDoesNotClearCurrentUser)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_NE(sys.getCurrentUser(), nullptr);

    sys.login("bad", "bad"); // fails
    // currentUser should still be valid from last successful login
    EXPECT_NE(sys.getCurrentUser(), nullptr);
}

// login: switch roles
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

/* ================= BOOK APPOINTMENT ================= */

// bookAppointment: currentUser == nullptr → false
TEST(SysFull, Book_NoCurrentUser)
{
    HospitalSystem sys;
    setupDoctor(sys);
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

// bookAppointment: role != "patient" (doctor logged in) → false
TEST(SysFull, Book_AsDoctor)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

// bookAppointment: role != "patient" (admin logged in) → false
TEST(SysFull, Book_AsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM"));
}

// bookAppointment: findDoctorById returns nullptr → false
TEST(SysFull, Book_DoctorNotFound)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(99999, "2026", "10AM"));
}

// bookAppointment: negative docId → false
TEST(SysFull, Book_NegativeDoctorId)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(-1, "2026", "10AM"));
}

// bookAppointment: valid → true, appointment in schedule
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

// bookAppointment: multiple bookings increment apptId
TEST(SysFull, Book_MultipleAppointments)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    for (int i = 0; i < 3; i++)
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
    EXPECT_EQ(sys.viewMyAppointments().size(), 3u);
}

// bookAppointment: findDoctorById loops over non-doctor users (admin, patient)
TEST(SysFull, Book_FindDoctorSkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);   // adds a non-doctor
    setupDoctor(sys);    // now doctor is last
    setupPatient(sys);   // another non-doctor after doctor

    sys.registerNewPatient("p2","p2@mail.com","456","011");
    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

/* ================= CANCEL APPOINTMENT ================= */

// cancelAppointmentPatient: currentUser == nullptr → false
TEST(SysFull, Cancel_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

// cancelAppointmentPatient: id not found in schedule → false (loop completes)
TEST(SysFull, Cancel_IdNotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(99999));
}

// cancelAppointmentPatient: id found BUT patientId != currentUser → false
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
    // appointmentId matches but patientId doesn't → falls through loop → false
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

// cancelAppointmentPatient: id+patient match, status==Scheduled → cancel → true
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
}

// cancelAppointmentPatient: status != Scheduled (already Cancelled) → false
TEST(SysFull, Cancel_AlreadyCancelled)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));  // first cancel → true
    EXPECT_FALSE(sys.cancelAppointmentPatient(id)); // second → status != Scheduled → false
}

// cancelAppointmentPatient: status != Scheduled (Completed) → false
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
    // status == Completed → cancel returns false
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

// cancelAppointmentPatient: multiple appts, cancel second one (loop iterates)
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

    EXPECT_TRUE(sys.cancelAppointmentPatient(id2)); // skips id1, cancels id2
    EXPECT_TRUE(sys.cancelAppointmentPatient(id1)); // cancels id1
}

/* ================= COMPLETE APPOINTMENT ================= */

// completeAppointmentDoctor: currentUser == nullptr → false
TEST(SysFull, Complete_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// completeAppointmentDoctor: currentUser != null but role != "doctor" → false
TEST(SysFull, Complete_AsPatient)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// completeAppointmentDoctor: role != "doctor" as admin → false
TEST(SysFull, Complete_AsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// completeAppointmentDoctor: doctor logged in, id not in schedule → false
TEST(SysFull, Complete_IdNotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(99999));
}

// completeAppointmentDoctor: id found but doctorId != currentUser → false
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
    // appointmentId matches d1 but doctor is d2 → false
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

// completeAppointmentDoctor: id+doctor match, status==Scheduled → complete → true
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
}

// completeAppointmentDoctor: status != Scheduled (already Completed) → false
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
    EXPECT_FALSE(sys.completeAppointmentDoctor(id)); // already Completed
}

// completeAppointmentDoctor: status == Cancelled → false
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
    // status == Cancelled → complete returns false
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

// completeAppointmentDoctor: multiple appts, complete the second (loop iterates)
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
    EXPECT_TRUE(sys.completeAppointmentDoctor(id2)); // skips id1, completes id2
    EXPECT_TRUE(sys.completeAppointmentDoctor(id1));
}

/* ================= VIEW MY APPOINTMENTS ================= */

// viewMyAppointments: currentUser == nullptr → empty
TEST(SysFull, ViewMine_NoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// viewMyAppointments: logged in, no appointments → empty
TEST(SysFull, ViewMine_EmptySchedule)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// viewMyAppointments: patientId matches → included
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

// viewMyAppointments: different patient → their appts not shown
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
    // p2 has no appointments → empty (patientId != currentUser condition → false)
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}

// viewMyAppointments: two patients, each sees only their own
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

/* ================= VIEW DOCTOR SCHEDULE ================= */

// viewDoctorSchedule: currentUser == nullptr → empty
TEST(SysFull, ViewDoc_NoUser)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

// viewDoctorSchedule: logged in as doctor, no appointments → empty
TEST(SysFull, ViewDoc_EmptySchedule)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

// viewDoctorSchedule: doctorId matches → included
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

// viewDoctorSchedule: appointment belongs to different doctor → not shown
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
    // appointment is for d1, d2 sees nothing
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}

// viewDoctorSchedule: two doctors each see only their own
TEST(SysFull, ViewDoc_TwoDoctorsIsolated)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.registerNewDoctor("d2", "d2@mail.com", "456", "neuro");
    setupPatient(sys);

    sys.login("p@mail.com", "123");

    int docId1 = -1, docId2 = -1;
    for (auto *u : sys.adminViewAllUsers())
    {
        if (u->get_role() == "doctor")
        {
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

/* ================= ADMIN VIEWS ================= */

TEST(SysFull, AdminViewUsers_OnlyAdmin)
{
    SysTest sys;
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 1u);
    EXPECT_EQ(users[0]->get_role(), "admin");
}

TEST(SysFull, AdminViewUsers_WithAll)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 3u);
}

TEST(SysFull, AdminViewAppointments_Empty)
{
    SysTest sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}

TEST(SysFull, AdminViewAppointments_AfterBooking)
{
    SysTest sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    sys.bookAppointment(docId, "2026", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 1u);
}

/* ================= FIND DOCTOR BRANCH COVERAGE ================= */

// findDoctorById: user is not a doctor → skips (role != "doctor" branch)
TEST(SysFull, FindDoctor_SkipsNonDoctors)
{
    HospitalSystem sys;
    // Only admin exists, no doctors
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    // docId 1 is admin, 2 is patient — both skipped
    EXPECT_FALSE(sys.bookAppointment(1, "2026", "10AM")); // admin id
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM")); // patient id
}

// findDoctorById: user IS doctor but id doesn't match → continues loop
TEST(SysFull, FindDoctor_WrongDoctorId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    // docId exists but we pass wrong id
    EXPECT_FALSE(sys.bookAppointment(99, "2026", "10AM"));
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
    EXPECT_FALSE(sys.cancelAppointmentPatient(id1)); // already cancelled

    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(id1)); // cancelled → false
    EXPECT_TRUE(sys.completeAppointmentDoctor(id2));  // scheduled → true
    EXPECT_FALSE(sys.completeAppointmentDoctor(id2)); // completed → false
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
