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

static void setupDoctorWithSpec(HospitalSystem &sys, const std::string &spec)
{
    sys.registerNewDoctor("DocSpec", "dspec@mail.com", "789", spec);
}

static void setupPatient(HospitalSystem &sys)
{
    sys.registerNewPatient("PatName", "p@mail.com", "123", "010");
}

static void setupPatientFull(HospitalSystem &sys, const std::string &name,
                              const std::string &email, const std::string &pass,
                              const std::string &phone)
{
    sys.registerNewPatient(name, email, pass, phone);
}

static int bookOne(HospitalSystem &sys, int docId)
{
    sys.bookAppointment(docId, "2026", "10AM");
    return sys.viewMyAppointments().back().get_AppointmentId();
}

static int bookOneDetailed(HospitalSystem &sys, int docId, 
                            const std::string &date, const std::string &time)
{
    sys.bookAppointment(docId, date, time);
    return sys.viewMyAppointments().back().get_AppointmentId();
}

/* ================= CONSTRUCTOR & BASIC STATE ================= */

// Constructor: verify initial state
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

// getCurrentUser: returns nullptr before any login
TEST(SysFull, GetCurrentUser_InitiallyNull)
{
    HospitalSystem sys;
    EXPECT_EQ(sys.getCurrentUser(), nullptr);
}

// getCurrentUser: returns valid pointer after login, stable across calls
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

/* ================= LOGIN - ENHANCED ================= */

// login: find_if returns end() → false (already covered, keep)
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

// login: whitespace-only credentials → false (treated as actual strings, no match)
TEST(SysFull, Login_WhitespaceOnly)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("   ", "   "));
    EXPECT_FALSE(sys.login("\t\n", "\t\n"));
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

// login: case sensitivity in email
TEST(SysFull, Login_CaseSensitiveEmail)
{
    HospitalSystem sys;
    setupPatient(sys);
    EXPECT_FALSE(sys.login("P@MAIL.COM", "123")); // uppercase should fail
}

// login: multiple failed attempts don't crash
TEST(SysFull, Login_MultipleFailures)
{
    HospitalSystem sys;
    for (int i = 0; i < 100; i++)
        EXPECT_FALSE(sys.login("bad", "bad"));
}

/* ================= REGISTRATION - NEW COVERAGE ================= */

// registerNewPatient: empty phone number
TEST(SysFull, RegisterPatient_EmptyPhone)
{
    HospitalSystem sys;
    sys.registerNewPatient("PatEmpty", "pempty@mail.com", "pass", "");
    EXPECT_TRUE(sys.login("pempty@mail.com", "pass"));
}

// FIXED: registerNewPatient: verify ID increments correctly
// Use same HospitalSystem instance for both registrations
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

// registerNewDoctor: empty specialty
TEST(SysFull, RegisterDoctor_EmptySpecialty)
{
    HospitalSystem sys;
    sys.registerNewDoctor("DocEmpty", "dempty@mail.com", "pass", "");
    EXPECT_TRUE(sys.login("dempty@mail.com", "pass"));
    EXPECT_EQ(sys.getCurrentUser()->get_role(), "doctor");
}

// registerNewDoctor: verify availability slots added
TEST(SysFull, RegisterDoctor_AvailabilityAdded)
{
    HospitalSystem sys;
    setupDoctor(sys);
    // Doctor constructor adds "10AM" and "11AM" via addAvailability
    // We can't directly test addAvailability without exposing it, 
    // but we verify doctor registration succeeds
    EXPECT_TRUE(sys.login("d@mail.com", "123"));
}

// FIXED: registerNewDoctor: multiple doctors with different specialties
// Use same HospitalSystem instance and count only on that instance
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

/* ================= BOOK APPOINTMENT - ENHANCED ================= */

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

// bookAppointment: doctor ID 0 (boundary, admin is ID 1)
TEST(SysFull, Book_DoctorIdZero)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(0, "2026", "10AM"));
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

    std::vector<int> ids;
    for (int i = 0; i < 3; i++) {
        EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
        ids.push_back(sys.viewMyAppointments().back().get_AppointmentId());
    }
    EXPECT_EQ(sys.viewMyAppointments().size(), 3u);
    // Verify IDs are strictly increasing
    EXPECT_LT(ids[0], ids[1]);
    EXPECT_LT(ids[1], ids[2]);
}

// bookAppointment: empty date
TEST(SysFull, Book_EmptyDate)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "", "10AM")); // system allows empty date
}

// bookAppointment: empty time
TEST(SysFull, Book_EmptyTime)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "")); // system allows empty time
}

// bookAppointment: both date and time empty
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

// bookAppointment: findDoctorById loops over non-doctor users (admin, patient)
TEST(SysFull, Book_FindDoctorSkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);   // adds a non-doctor (ID 2)
    setupDoctor(sys);    // now doctor is last (ID 3)
    setupPatient(sys);   // another non-doctor after doctor (ID 4)

    sys.registerNewPatient("p2","p2@mail.com","456","011");
    sys.login("p@mail.com", "123");

    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    EXPECT_TRUE(sys.bookAppointment(docId, "2026", "10AM"));
}

// bookAppointment: book with different times
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

/* ================= CANCEL APPOINTMENT - ENHANCED ================= */

// cancelAppointmentPatient: currentUser == nullptr → false
TEST(SysFull, Cancel_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}

// cancelAppointmentPatient: id not found in schedule → false
TEST(SysFull, Cancel_IdNotFound)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(99999));
}

// cancelAppointmentPatient: negative id
TEST(SysFull, Cancel_NegativeId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(-1));
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

    // Verify status is Cancelled via admin view
    // Need to access through sys since st is separate instance
    // Actually, let's verify through viewMyAppointments
    sys.login("p@mail.com", "123");
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);
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
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
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
    EXPECT_FALSE(sys.cancelAppointmentPatient(id));
}

// cancelAppointmentPatient: multiple appts, cancel second one
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

// cancelAppointmentPatient: cancel first appointment, second remains scheduled
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

    // Find remaining scheduled one
    bool foundScheduled = false;
    for (const auto &a : appts) {
        if (a.get_AppointmentId() == id2 && a.get_Status() == Status::Scheduled)
            foundScheduled = true;
    }
    EXPECT_TRUE(foundScheduled);
}

/* ================= COMPLETE APPOINTMENT - ENHANCED ================= */

// completeAppointmentDoctor: currentUser == nullptr → false
TEST(SysFull, Complete_NoCurrentUser)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}

// completeAppointmentDoctor: currentUser != null but role != "doctor" → false (patient)
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

// completeAppointmentDoctor: negative id
TEST(SysFull, Complete_NegativeId)
{
    HospitalSystem sys;
    setupDoctor(sys);
    sys.login("d@mail.com", "123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(-1));
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

    // Verify status
    auto schedule = sys.viewDoctorSchedule();
    EXPECT_EQ(schedule.size(), 1u);
    EXPECT_EQ(schedule[0].get_Status(), Status::Completed);
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
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
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
    EXPECT_FALSE(sys.completeAppointmentDoctor(id));
}

// completeAppointmentDoctor: multiple appts, complete the second
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

// completeAppointmentDoctor: complete first, second remains scheduled
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

/* ================= VIEW MY APPOINTMENTS - ENHANCED ================= */

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

// viewMyAppointments: after cancellation, still shows in list (status changed)
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

// viewMyAppointments: after completion, still shows in list (status changed)
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

/* ================= VIEW DOCTOR SCHEDULE - ENHANCED ================= */

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

// viewDoctorSchedule: shows cancelled appointments
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

// viewDoctorSchedule: shows completed appointments
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

/* ================= ADMIN VIEWS - ENHANCED ================= */

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

/* ================= FIND DOCTOR BRANCH COVERAGE - ENHANCED ================= */

// findDoctorById: user is not a doctor → skips
TEST(SysFull, FindDoctor_SkipsNonDoctors)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
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
    EXPECT_FALSE(sys.bookAppointment(99, "2026", "10AM"));
}

// findDoctorById: doctor ID 0 (no user should have ID 0)
TEST(SysFull, FindDoctor_IdZero)
{
    HospitalSystem sys;
    setupPatient(sys);
    sys.login("p@mail.com", "123");
    EXPECT_FALSE(sys.bookAppointment(0, "2026", "10AM"));
}

// findDoctorById: multiple doctors, find second one
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

/* ================= APPOINTMENT ID MANAGEMENT ================= */

// Verify appointment IDs are unique and incrementing
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
        EXPECT_EQ(ids.count(id), 0u); // unique
        ids.insert(id);
    }
}

/* ================= USER ID MANAGEMENT ================= */

// Verify user IDs are unique
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
        EXPECT_EQ(ids.count(u->get_id()), 0u); // unique
        ids.insert(u->get_id());
    }
}

/* ================= CHAOS / STRESS - ENHANCED ================= */

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

// Stress test: many appointments
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

// FIXED: Stress test: many users
// Use same HospitalSystem instance, not separate SysTest
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
    // Admin + N patients
    EXPECT_EQ(users.size(), static_cast<size_t>(N + 1));
}

// Stress test: rapid login/logout switching
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

// Full workflow: book → view → cancel → view → admin view
TEST(SysFull, Workflow_BookCancelAdmin)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);

    // Patient sees it
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);

    // Cancel it
    EXPECT_TRUE(sys.cancelAppointmentPatient(id));

    // Patient still sees it (as cancelled)
    auto appts = sys.viewMyAppointments();
    EXPECT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_Status(), Status::Cancelled);

    // Admin sees cancelled status
    auto allAppts = sys.adminViewAllAppointments();
    EXPECT_EQ(allAppts.size(), 1u);
    EXPECT_EQ(allAppts[0].get_Status(), Status::Cancelled);
}

// Full workflow: book → complete → verify both views
TEST(SysFull, Workflow_BookCompleteVerify)
{
    HospitalSystem sys;
    setupDoctor(sys);
    setupPatient(sys);

    sys.login("p@mail.com", "123");
    int docId = getDoctorId(sys);
    ASSERT_NE(docId, -1);
    int id = bookOne(sys, docId);

    // Doctor completes
    sys.login("d@mail.com", "123");
    EXPECT_TRUE(sys.completeAppointmentDoctor(id));

    // Doctor sees completed
    auto docSchedule = sys.viewDoctorSchedule();
    EXPECT_EQ(docSchedule.size(), 1u);
    EXPECT_EQ(docSchedule[0].get_Status(), Status::Completed);

    // Patient sees completed
    sys.login("p@mail.com", "123");
    auto patAppts = sys.viewMyAppointments();
    EXPECT_EQ(patAppts.size(), 1u);
    EXPECT_EQ(patAppts[0].get_Status(), Status::Completed);
}

// Workflow: multiple patients, one doctor
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
    int id1 = bookOne(sys, docId);

    sys.login("p2@mail.com", "456");
    int id2 = bookOne(sys, docId);

    sys.login("p3@mail.com", "789");
    int id3 = bookOne(sys, docId);

    // Doctor sees all 3
    sys.login("d@mail.com", "123");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 3u);

    // Each patient sees only their own
    sys.login("p@mail.com", "123");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);

    sys.login("p2@mail.com", "456");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);

    sys.login("p3@mail.com", "789");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}
