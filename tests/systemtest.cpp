#include <gtest/gtest.h>
#include "system.h"
#include "user.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"
#include "Appointment.h"
 
// ══════════════════════════════════════════════════════════════
//  APPOINTMENT CLASS
// ══════════════════════════════════════════════════════════════
 
TEST(AppointmentTest, ConstructorStoresAllFields)
{
    Appointment a(1, 10, 20, "Alice", "Dr. Bob", "2025-01-01", "10AM", "Scheduled");
    EXPECT_EQ(a.get_AppointmentId(), 1);
    EXPECT_EQ(a.get_PatientId(),     10);
    EXPECT_EQ(a.get_DoctorId(),      20);
    EXPECT_EQ(a.get_PatientName(),   "Alice");
    EXPECT_EQ(a.get_DoctorName(),    "Dr. Bob");
    EXPECT_EQ(a.get_Date(),          "2025-01-01");
    EXPECT_EQ(a.get_Time(),          "10AM");
    EXPECT_EQ(a.get_Status(),        "Scheduled");
}
 
TEST(AppointmentTest, CancelSetsStatusCancelled)
{
    Appointment a(2, 10, 20, "Alice", "Dr. Bob", "2025-01-01", "10AM", "Scheduled");
    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");
}
 
TEST(AppointmentTest, CompleteSetsStatusCompleted)
{
    Appointment a(3, 10, 20, "Alice", "Dr. Bob", "2025-01-01", "10AM", "Scheduled");
    a.complete();
    EXPECT_EQ(a.get_Status(), "Completed");
}
 
// ══════════════════════════════════════════════════════════════
//  USER / ADMIN / DOCTOR / PATIENT CLASSES
// ══════════════════════════════════════════════════════════════
 
TEST(UserTest, AuthenticateSucceedsCorrectCredentials)
{
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_TRUE(u.Authenticate("admin@mail.com", "admin123"));
}
 
TEST(UserTest, AuthenticateFailsWrongPassword)
{
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_FALSE(u.Authenticate("admin@mail.com", "wrong"));
}
 
TEST(UserTest, AuthenticateFailsWrongEmail)
{
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_FALSE(u.Authenticate("other@mail.com", "admin123"));
}
 
TEST(UserTest, GettersReturnCorrectValues)
{
    Admin u(5, "AdminName", "a@b.com", "pass");
    EXPECT_EQ(u.get_id(),    5);
    EXPECT_EQ(u.get_name(),  "AdminName");
    EXPECT_EQ(u.get_email(), "a@b.com");
    EXPECT_EQ(u.get_role(),  "admin");
}
 
TEST(DoctorTest, RoleIsDoctor)
{
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_role(), "doctor");
}
 
TEST(DoctorTest, SpecializationStoredCorrectly)
{
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_specialization(), "Cardiology");
}
 
TEST(DoctorTest, AddAvailabilityAppendsSlots)
{
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    d.addAvailability("10AM");
    d.addAvailability("2PM");
    auto slots = d.getAvailability();
    ASSERT_EQ(slots.size(), 2u);
    EXPECT_EQ(slots[0], "10AM");
    EXPECT_EQ(slots[1], "2PM");
}
 
TEST(PatientTest, RoleIsPatient)
{
    patient p(1, "Alice", "alice@mail.com", "pass", "0123456789");
    EXPECT_EQ(p.get_role(), "patient");
}
 
TEST(PatientTest, PhoneStoredCorrectly)
{
    patient p(1, "Alice", "alice@mail.com", "pass", "0123456789");
    EXPECT_EQ(p.get_phone(), "0123456789");
}
 
// ══════════════════════════════════════════════════════════════
//  HELPER – registers 1 doctor + 1 patient, returns doctorId
// ══════════════════════════════════════════════════════════════
 
static int setupOneDoctorOnePatient(HospitalSystem &sys)
{
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    return sys.adminViewAllUsers()[1]->get_id(); // doctor is always index 1
}
 
// ══════════════════════════════════════════════════════════════
//  CONSTRUCTOR  (lines 9-15)
// ══════════════════════════════════════════════════════════════
 
// lines 11-14: nextUserId=1, nextApptId=1, currentUser=nullptr, push Admin
TEST(HospitalSystemTest, ConstructorSeededAdminDetails)
{
    HospitalSystem sys;
    auto users = sys.adminViewAllUsers();
    ASSERT_EQ(users.size(), 1u);
    EXPECT_EQ(users[0]->get_role(),  "admin");
    EXPECT_EQ(users[0]->get_name(),  "admin");
    EXPECT_EQ(users[0]->get_email(), "admin@mail.com");
    EXPECT_EQ(users[0]->get_id(),    1);
}
 
TEST(HospitalSystemTest, ConstructorStartsWithNoAppointments)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}
 
// ══════════════════════════════════════════════════════════════
//  DESTRUCTOR  (lines 17-21)
// ══════════════════════════════════════════════════════════════
 
// lines 19-20: for-loop deletes every user pointer
TEST(HospitalSystemTest, DestructorFreesMultipleUsers)
{
    HospitalSystem *sys = new HospitalSystem();
    sys->registerNewDoctor("Dr. X", "x@mail.com", "ENT");
    sys->registerNewPatient("Bob", "bob@mail.com", "111");
    EXPECT_NO_THROW(delete sys);
}
 
// ══════════════════════════════════════════════════════════════
//  LOGIN  (lines 33-44)
// ══════════════════════════════════════════════════════════════
 
// lines 37-40: Authenticate true -> set currentUser, return true
TEST(HospitalSystemTest, LoginSucceedsAdmin)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
}
 
// line 43: all iterations fail -> return false
TEST(HospitalSystemTest, LoginFailsWrongPassword)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("admin@mail.com", "wrong"));
}
 
TEST(HospitalSystemTest, LoginFailsWrongEmail)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("nobody@mail.com", "admin123"));
}
 
TEST(HospitalSystemTest, LoginSucceedsRegisteredPatient)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    EXPECT_TRUE(sys.login("alice@mail.com", "default"));
}
 
TEST(HospitalSystemTest, LoginSucceedsRegisteredDoctor)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_TRUE(sys.login("smith@mail.com", "default"));
}
 
// Second login overwrites currentUser (line 39 hit twice)
TEST(HospitalSystemTest, LoginOverwritesCurrentUser)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    EXPECT_TRUE(sys.login("alice@mail.com", "default"));
    EXPECT_TRUE(sys.login("bob@mail.com",   "default"));
    EXPECT_TRUE(sys.viewMyAppointments().empty()); // Bob has no appts
}
 
// ══════════════════════════════════════════════════════════════
//  REGISTER NEW PATIENT  (lines 46-50)
// ══════════════════════════════════════════════════════════════
 
// line 48: new patient(nextUserId++, ...) ; line 49: push_back
TEST(HospitalSystemTest, RegisterPatientAddsToUserList)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    EXPECT_EQ(sys.adminViewAllUsers().size(), 2u);
}
 
TEST(HospitalSystemTest, RegisterPatientCorrectRole)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    EXPECT_EQ(sys.adminViewAllUsers()[1]->get_role(), "patient");
}
 
TEST(HospitalSystemTest, RegisterPatientCorrectName)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    EXPECT_EQ(sys.adminViewAllUsers()[1]->get_name(), "Alice");
}
 
TEST(HospitalSystemTest, RegisterPatientCorrectEmail)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    EXPECT_EQ(sys.adminViewAllUsers()[1]->get_email(), "alice@mail.com");
}
 
TEST(HospitalSystemTest, RegisterMultiplePatientsUniqueIds)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    auto users = sys.adminViewAllUsers();
    ASSERT_EQ(users.size(), 3u);
    EXPECT_NE(users[1]->get_id(), users[2]->get_id());
}
 
TEST(HospitalSystemTest, RegisterPatientPasswordIsDefault)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    EXPECT_TRUE(sys.login("alice@mail.com", "default"));
}
 
// ══════════════════════════════════════════════════════════════
//  REGISTER NEW DOCTOR  (lines 52-58)
// ══════════════════════════════════════════════════════════════
 
// line 54: Doctor created ; lines 55-56: addAvailability x2 ; line 57: push_back
TEST(HospitalSystemTest, RegisterDoctorAddsToUserList)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_EQ(sys.adminViewAllUsers().size(), 2u);
}
 
TEST(HospitalSystemTest, RegisterDoctorCorrectRole)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_EQ(sys.adminViewAllUsers()[1]->get_role(), "doctor");
}
 
TEST(HospitalSystemTest, RegisterDoctorHasTwoDefaultSlots)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    Doctor *d = dynamic_cast<Doctor *>(sys.adminViewAllUsers()[1]);
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d->getAvailability().size(), 2u);
    EXPECT_EQ(d->getAvailability()[0], "10AM");
    EXPECT_EQ(d->getAvailability()[1], "11AM");
}
 
TEST(HospitalSystemTest, RegisterDoctorPasswordIsDefault)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_TRUE(sys.login("smith@mail.com", "default"));
}
 
TEST(HospitalSystemTest, RegisterMultipleDoctorsUniqueIds)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewDoctor("Dr. Jones", "jones@mail.com", "Neurology");
    auto users = sys.adminViewAllUsers();
    EXPECT_NE(users[1]->get_id(), users[2]->get_id());
}
 
// ══════════════════════════════════════════════════════════════
//  CHECK CONFLICT  (lines 60-71)
// ══════════════════════════════════════════════════════════════
 
// line 70: empty schedule -> return false
TEST(HospitalSystemTest, CheckConflictFalseEmptySchedule)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.checkConflict(99, "2025-01-01", "10AM"));
}
 
// lines 64-68: all four conditions true -> line 68 return true
TEST(HospitalSystemTest, CheckConflictTrueAfterBooking)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_TRUE(sys.checkConflict(docId, "2025-01-01", "10AM"));
}
 
// line 64: doctorId mismatch -> short-circuit, fall to line 70
TEST(HospitalSystemTest, CheckConflictFalseDifferentDoctor)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_FALSE(sys.checkConflict(docId + 99, "2025-01-01", "10AM"));
}
 
// line 65: date mismatch -> short-circuit
TEST(HospitalSystemTest, CheckConflictFalseDifferentDate)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-02", "10AM"));
}
 
// line 66: time mismatch -> short-circuit
TEST(HospitalSystemTest, CheckConflictFalseDifferentTime)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-01", "11AM"));
}
 
// line 67: status != "Scheduled" -> short-circuit (cancelled appt is not a conflict)
TEST(HospitalSystemTest, CheckConflictFalseAfterCancellation)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-01", "10AM"));
}
 
// ══════════════════════════════════════════════════════════════
//  BOOK APPOINTMENT  (lines 73-86)
// ══════════════════════════════════════════════════════════════
 
// line 75-76: !currentUser -> return false
TEST(HospitalSystemTest, BookFailsNotLoggedIn)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_FALSE(sys.bookAppointment(2, "2025-01-01", "10AM"));
}
 
// line 75-76: role != "patient" (admin)
TEST(HospitalSystemTest, BookFailsLoggedInAsAdmin)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
// line 75-76: role != "patient" (doctor)
TEST(HospitalSystemTest, BookFailsLoggedInAsDoctor)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
// line 77-78: checkConflict true -> return false
TEST(HospitalSystemTest, BookFailsOnConflict)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE( sys.bookAppointment(docId, "2025-01-01", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
// line 79-81: findDoctorById returns nullptr -> return false
TEST(HospitalSystemTest, BookFailsNonExistentDoctor)
{
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.bookAppointment(9999, "2025-01-01", "10AM"));
}
 
// lines 82-85: Appointment created and pushed -> return true
TEST(HospitalSystemTest, BookSucceedsHappyPath)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAddsToMasterSchedule)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 1u);
}
 
TEST(HospitalSystemTest, BookSetsCorrectPatientAndDoctorNames)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    auto &a = sys.adminViewAllAppointments()[0];
    EXPECT_EQ(a.get_PatientName(), "Alice");
    EXPECT_EQ(a.get_DoctorName(),  "Dr. Smith");
}
 
TEST(HospitalSystemTest, BookSetsStatusScheduled)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Scheduled");
}
 
TEST(HospitalSystemTest, BookIncrementsAppointmentId)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts[0].get_AppointmentId(), 1);
    EXPECT_EQ(appts[1].get_AppointmentId(), 2);
}
 
TEST(HospitalSystemTest, BookStoresCorrectDateAndTime)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-06-15", "11AM");
    auto &a = sys.adminViewAllAppointments()[0];
    EXPECT_EQ(a.get_Date(), "2025-06-15");
    EXPECT_EQ(a.get_Time(), "11AM");
}
 
// ══════════════════════════════════════════════════════════════
//  CANCEL APPOINTMENT PATIENT  (lines 88-103)
// ══════════════════════════════════════════════════════════════
 
// line 90-91: !currentUser -> return false
TEST(HospitalSystemTest, CancelFailsNotLoggedIn)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// line 90-91: role != "patient" (admin)
TEST(HospitalSystemTest, CancelFailsLoggedInAsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// line 90-91: role != "patient" (doctor)
TEST(HospitalSystemTest, CancelFailsLoggedInAsDoctor)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// line 102: loop exhausted, no appt found -> return false
TEST(HospitalSystemTest, CancelFailsNonExistentAppt)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}
 
// line 94: appt id mismatch -> loop continues then return false
TEST(HospitalSystemTest, CancelFailsWrongApptId)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM"); // appt id = 1
    EXPECT_FALSE(sys.cancelAppointmentPatient(2));    // id 2 does not exist
}
 
// line 95: patientId mismatch -> loop continues
TEST(HospitalSystemTest, CancelFailsAnotherPatientsAppt)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.registerNewPatient("Bob", "bob@mail.com", "222");
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("bob@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// line 96: status != "Scheduled" (already cancelled) -> loop continues
TEST(HospitalSystemTest, CancelFailsAlreadyCancelled)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// lines 98-99: cancel() called -> return true
TEST(HospitalSystemTest, CancelSucceedsOwnScheduledAppt)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_TRUE(sys.cancelAppointmentPatient(1));
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Cancelled");
}
 
TEST(HospitalSystemTest, CancelMultipleApptsSamePatient)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM"); // id=1
    sys.bookAppointment(docId, "2025-01-01", "11AM"); // id=2
    EXPECT_TRUE(sys.cancelAppointmentPatient(1));
    EXPECT_TRUE(sys.cancelAppointmentPatient(2));
}
 
// ══════════════════════════════════════════════════════════════
//  VIEW MY APPOINTMENTS  (lines 105-116)
// ══════════════════════════════════════════════════════════════
 
// line 108-109: !currentUser -> return empty result
TEST(HospitalSystemTest, ViewMyApptsEmptyNotLoggedIn)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
// line 108-109: role != "patient" (admin)
TEST(HospitalSystemTest, ViewMyApptsEmptyAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
// line 108-109: role != "patient" (doctor)
TEST(HospitalSystemTest, ViewMyApptsEmptyDoctor)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
// lines 110-114: loop iterates, no patientId match -> line 115 return empty
TEST(HospitalSystemTest, ViewMyApptsEmptyWhenNoBookings)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
// lines 112-113: patientId matches -> push_back ; line 115: return result
TEST(HospitalSystemTest, ViewMyApptsReturnsOwnAppointments)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    auto appts = sys.viewMyAppointments();
    ASSERT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_PatientName(), "Alice");
}
 
// filter: two patients, each sees only their own (exercises line 112 false branch)
TEST(HospitalSystemTest, ViewMyApptsFiltersOtherPatient)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.registerNewPatient("Bob", "bob@mail.com", "222");
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("bob@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
 
    sys.login("alice@mail.com", "default");
    auto appts = sys.viewMyAppointments();
    ASSERT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_PatientName(), "Alice");
}
 
// cancelled appointments still appear in patient's list
TEST(HospitalSystemTest, ViewMyApptsIncludesCancelledOnes)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}
 
// multiple bookings all returned (exercises line 115 with result.size > 1)
TEST(HospitalSystemTest, ViewMyApptsReturnsMultiple)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
    EXPECT_EQ(sys.viewMyAppointments().size(), 2u);
}
 
// ══════════════════════════════════════════════════════════════
//  VIEW DOCTOR SCHEDULE  (lines 118-129)
// ══════════════════════════════════════════════════════════════
 
// line 121-122: !currentUser -> return empty
TEST(HospitalSystemTest, ViewDoctorScheduleEmptyNotLoggedIn)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
// line 121-122: role != "doctor" (patient)
TEST(HospitalSystemTest, ViewDoctorScheduleEmptyPatient)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
// line 121-122: role != "doctor" (admin)
TEST(HospitalSystemTest, ViewDoctorScheduleEmptyAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
// lines 123-127: loop runs, no doctorId match -> line 128 return empty
TEST(HospitalSystemTest, ViewDoctorScheduleEmptyNoBookings)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
// lines 125-126: doctorId matches -> push_back ; line 128 return result
TEST(HospitalSystemTest, ViewDoctorScheduleReturnsOwnAppts)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    auto sched = sys.viewDoctorSchedule();
    ASSERT_EQ(sched.size(), 1u);
    EXPECT_EQ(sched[0].get_DoctorName(), "Dr. Smith");
}
 
// two doctors – each sees only own (exercises line 125 false branch)
TEST(HospitalSystemTest, ViewDoctorScheduleFiltersOtherDoctor)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewDoctor("Dr. Jones", "jones@mail.com", "Neurology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int smithId = sys.adminViewAllUsers()[1]->get_id();
    int jonesId = sys.adminViewAllUsers()[2]->get_id();
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(smithId, "2025-01-01", "10AM");
    sys.bookAppointment(jonesId, "2025-01-02", "10AM");
 
    sys.login("smith@mail.com", "default");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
 
    sys.login("jones@mail.com", "default");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
}
 
// multiple patients booking same doctor (result.size > 1 on line 128)
TEST(HospitalSystemTest, ViewDoctorScheduleMultiplePatients)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.registerNewPatient("Bob", "bob@mail.com", "222");
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("bob@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
 
    sys.login("smith@mail.com", "default");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 2u);
}
 
// ══════════════════════════════════════════════════════════════
//  COMPLETE APPOINTMENT DOCTOR  (lines 131-146)
// ══════════════════════════════════════════════════════════════
 
// line 133-134: !currentUser -> return false
TEST(HospitalSystemTest, CompleteFailsNotLoggedIn)
{
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// line 133-134: role != "doctor" (patient)
TEST(HospitalSystemTest, CompleteFailsLoggedInAsPatient)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// line 133-134: role != "doctor" (admin)
TEST(HospitalSystemTest, CompleteFailsLoggedInAsAdmin)
{
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// line 145: loop exhausted (appt id not found)
TEST(HospitalSystemTest, CompleteFailsNonExistentAppt)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}
 
// line 137: appt id mismatch -> loop continues
TEST(HospitalSystemTest, CompleteFailsWrongApptId)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM"); // id=1
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(2));   // id 2 does not exist
}
 
// line 138: doctorId mismatch -> loop continues
TEST(HospitalSystemTest, CompleteFailsAnotherDoctorsAppt)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewDoctor("Dr. Jones", "jones@mail.com", "Neurology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int smithId = sys.adminViewAllUsers()[1]->get_id();
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(smithId, "2025-01-01", "10AM");
 
    sys.login("jones@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// line 139: status != "Scheduled" (already completed)
TEST(HospitalSystemTest, CompleteFailsAlreadyCompleted)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    sys.completeAppointmentDoctor(1);
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// line 139: status != "Scheduled" (cancelled)
TEST(HospitalSystemTest, CompleteFailsCancelledAppt)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// lines 141-142: complete() called -> return true
TEST(HospitalSystemTest, CompleteSucceedsOwnScheduledAppt)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.completeAppointmentDoctor(1));
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Completed");
}
 
TEST(HospitalSystemTest, CompleteMultipleApptsSameDoctor)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM"); // id=1
    sys.bookAppointment(docId, "2025-01-01", "11AM"); // id=2
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.completeAppointmentDoctor(1));
    EXPECT_TRUE(sys.completeAppointmentDoctor(2));
}
 
// ══════════════════════════════════════════════════════════════
//  ADMIN VIEW ALL APPOINTMENTS  (lines 148-151)
// ══════════════════════════════════════════════════════════════
 
// line 150: return masterSchedule (empty)
TEST(HospitalSystemTest, AdminViewAllApptsEmptyInitially)
{
    HospitalSystem sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}
 
// line 150: return masterSchedule (with entries)
TEST(HospitalSystemTest, AdminViewAllApptsReturnsAll)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.registerNewPatient("Bob", "bob@mail.com", "222");
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("bob@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
 
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 2u);
}
 
TEST(HospitalSystemTest, AdminViewAllApptsContainsMixedStatuses)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM"); // scheduled
    sys.bookAppointment(docId, "2025-01-01", "11AM"); // will cancel
    sys.cancelAppointmentPatient(2);
    auto appts = sys.adminViewAllAppointments();
    ASSERT_EQ(appts.size(), 2u);
    EXPECT_EQ(appts[0].get_Status(), "Scheduled");
    EXPECT_EQ(appts[1].get_Status(), "Cancelled");
}
 
// ══════════════════════════════════════════════════════════════
//  ADMIN VIEW ALL USERS  (lines 153-156)
// ══════════════════════════════════════════════════════════════
 
// line 155: return allUsers (size 1 – only admin)
TEST(HospitalSystemTest, AdminViewAllUsersOnlyAdminInitially)
{
    HospitalSystem sys;
    EXPECT_EQ(sys.adminViewAllUsers().size(), 1u);
}
 
// line 155: return allUsers (size 3)
TEST(HospitalSystemTest, AdminViewAllUsersGrowsWithRegistrations)
{
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    EXPECT_EQ(sys.adminViewAllUsers().size(), 3u);
}
 
// ══════════════════════════════════════════════════════════════
//  FIND DOCTOR BY ID – private, exercised via bookAppointment
//  (lines 23-31)
// ══════════════════════════════════════════════════════════════
 
// lines 27-28: role=="doctor" && id matches -> return Doctor ptr (book succeeds)
TEST(HospitalSystemTest, FindDoctorByIdSucceeds)
{
    HospitalSystem sys;
    int docId = setupOneDoctorOnePatient(sys);
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
// line 30: loop exhausted, no doctor found -> nullptr (patient id used)
TEST(HospitalSystemTest, FindDoctorByIdFailsPatientId)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    int patientId = sys.adminViewAllUsers()[2]->get_id();
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.bookAppointment(patientId, "2025-01-01", "10AM"));
}
 
// line 27: role check fails (admin id) -> skip, continue to nullptr
TEST(HospitalSystemTest, FindDoctorByIdFailsAdminId)
{
    HospitalSystem sys;
    setupOneDoctorOnePatient(sys);
    int adminId = sys.adminViewAllUsers()[0]->get_id();
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.bookAppointment(adminId, "2025-01-01", "10AM"));
}
 
