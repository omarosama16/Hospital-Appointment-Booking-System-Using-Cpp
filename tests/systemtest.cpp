#include <gtest/gtest.h>
#include "system.h"
#include "user.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"
#include "Appointment.h"
 
// ─────────────────────────────────────────────────────────────
//  APPOINTMENT CLASS TESTS
// ─────────────────────────────────────────────────────────────
 
TEST(AppointmentTest, ConstructorStoresAllFields) {
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
 
TEST(AppointmentTest, CancelChangesStatusToCancelled) {
    Appointment a(2, 10, 20, "Alice", "Dr. Bob", "2025-01-01", "10AM", "Scheduled");
    a.cancel();
    EXPECT_EQ(a.get_Status(), "Cancelled");
}
 
TEST(AppointmentTest, CompleteChangesStatusToCompleted) {
    Appointment a(3, 10, 20, "Alice", "Dr. Bob", "2025-01-01", "10AM", "Scheduled");
    a.complete();
    EXPECT_EQ(a.get_Status(), "Completed");
}
 
// ─────────────────────────────────────────────────────────────
//  USER / DOCTOR / PATIENT / ADMIN CLASS TESTS
// ─────────────────────────────────────────────────────────────
 
TEST(UserTest, AuthenticateSucceedsWithCorrectCredentials) {
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_TRUE(u.Authenticate("admin@mail.com", "admin123"));
}
 
TEST(UserTest, AuthenticateFailsWithWrongPassword) {
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_FALSE(u.Authenticate("admin@mail.com", "wrong"));
}
 
TEST(UserTest, AuthenticateFailsWithWrongEmail) {
    Admin u(1, "admin", "admin@mail.com", "admin123");
    EXPECT_FALSE(u.Authenticate("other@mail.com", "admin123"));
}
 
TEST(UserTest, GettersReturnCorrectValues) {
    Admin u(5, "AdminName", "a@b.com", "pass");
    EXPECT_EQ(u.get_id(),    5);
    EXPECT_EQ(u.get_name(),  "AdminName");
    EXPECT_EQ(u.get_email(), "a@b.com");
    EXPECT_EQ(u.get_role(),  "admin");
}
 
TEST(DoctorTest, RoleIsDoctor) {
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_role(), "doctor");
}
 
TEST(DoctorTest, SpecializationStoredCorrectly) {
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    EXPECT_EQ(d.get_specialization(), "Cardiology");
}
 
TEST(DoctorTest, AddAvailabilityAppendsSlots) {
    Doctor d(1, "Dr. Smith", "smith@mail.com", "pass", "Cardiology");
    d.addAvailability("10AM");
    d.addAvailability("2PM");
    auto slots = d.getAvailability();
    ASSERT_EQ(slots.size(), 2u);
    EXPECT_EQ(slots[0], "10AM");
    EXPECT_EQ(slots[1], "2PM");
}
 
TEST(PatientTest, RoleIsPatient) {
    patient p(1, "Alice", "alice@mail.com", "pass", "0123456789");
    EXPECT_EQ(p.get_role(), "patient");
}
 
TEST(PatientTest, PhoneStoredCorrectly) {
    patient p(1, "Alice", "alice@mail.com", "pass", "0123456789");
    EXPECT_EQ(p.get_phone(), "0123456789");
}
 
// ─────────────────────────────────────────────────────────────
//  HOSPITALSYSTEM — CONSTRUCTOR & DESTRUCTOR
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, ConstructorCreatesAdminUser) {
    HospitalSystem sys;
    // adminViewAllUsers includes the seeded admin
    auto users = sys.adminViewAllUsers();
    ASSERT_EQ(users.size(), 1u);
    EXPECT_EQ(users[0]->get_role(),  "admin");
    EXPECT_EQ(users[0]->get_name(),  "admin");
    EXPECT_EQ(users[0]->get_email(), "admin@mail.com");
}
 
TEST(HospitalSystemTest, ConstructorStartsWithNoAppointments) {
    HospitalSystem sys;
    EXPECT_TRUE(sys.adminViewAllAppointments().empty());
}
 
// ─────────────────────────────────────────────────────────────
//  LOGIN
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, LoginSucceedsForAdmin) {
    HospitalSystem sys;
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
}
 
TEST(HospitalSystemTest, LoginFailsWithWrongPassword) {
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("admin@mail.com", "wrongpass"));
}
 
TEST(HospitalSystemTest, LoginFailsWithWrongEmail) {
    HospitalSystem sys;
    EXPECT_FALSE(sys.login("nobody@mail.com", "admin123"));
}
 
TEST(HospitalSystemTest, LoginSucceedsForRegisteredPatient) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    EXPECT_TRUE(sys.login("alice@mail.com", "default"));
}
 
TEST(HospitalSystemTest, LoginSucceedsForRegisteredDoctor) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_TRUE(sys.login("smith@mail.com", "default"));
}
 
// ─────────────────────────────────────────────────────────────
//  REGISTER NEW PATIENT
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, RegisterNewPatientAddsToUserList) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users.size(), 2u); // admin + patient
}
 
TEST(HospitalSystemTest, RegisteredPatientHasCorrectRole) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users[1]->get_role(), "patient");
}
 
TEST(HospitalSystemTest, RegisteredPatientHasCorrectName) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users[1]->get_name(), "Alice");
}
 
TEST(HospitalSystemTest, RegisteredPatientHasCorrectEmail) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "0100000001");
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users[1]->get_email(), "alice@mail.com");
}
 
TEST(HospitalSystemTest, RegisterMultiplePatientsIncreasesIdEachTime) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    auto users = sys.adminViewAllUsers();
    ASSERT_EQ(users.size(), 3u);
    EXPECT_NE(users[1]->get_id(), users[2]->get_id());
}
 
// ─────────────────────────────────────────────────────────────
//  REGISTER NEW DOCTOR
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, RegisterNewDoctorAddsToUserList) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    EXPECT_EQ(sys.adminViewAllUsers().size(), 2u);
}
 
TEST(HospitalSystemTest, RegisteredDoctorHasCorrectRole) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    auto users = sys.adminViewAllUsers();
    EXPECT_EQ(users[1]->get_role(), "doctor");
}
 
TEST(HospitalSystemTest, RegisteredDoctorHasTwoDefaultAvailabilitySlots) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    // login as doctor to verify via cast
    sys.login("smith@mail.com", "default");
    auto users = sys.adminViewAllUsers();
    Doctor *d = dynamic_cast<Doctor *>(users[1]);
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d->getAvailability().size(), 2u);
    EXPECT_EQ(d->getAvailability()[0], "10AM");
    EXPECT_EQ(d->getAvailability()[1], "11AM");
}
 
// ─────────────────────────────────────────────────────────────
//  CHECK CONFLICT
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseWhenScheduleEmpty) {
    HospitalSystem sys;
    EXPECT_FALSE(sys.checkConflict(99, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, CheckConflictReturnsTrueAfterBooking) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    auto users = sys.adminViewAllUsers();
    int docId = users[1]->get_id(); // doctor is index 1
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_TRUE(sys.checkConflict(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseForDifferentDate) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-02", "10AM"));
}
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseForDifferentTime) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-01", "11AM"));
}
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseAfterCancellation) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    // Cancelled appointment no longer conflicts
    EXPECT_FALSE(sys.checkConflict(docId, "2025-01-01", "10AM"));
}
 
// ─────────────────────────────────────────────────────────────
//  BOOK APPOINTMENT
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, BookAppointmentFailsWhenNotLoggedIn) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentFailsWhenLoggedInAsAdmin) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("admin@mail.com", "admin123");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentFailsWhenLoggedInAsDoctor) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("smith@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentFailsForNonExistentDoctor) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.bookAppointment(9999, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentSucceedsForPatientWithValidDoctor) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_TRUE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentAddsToMasterSchedule) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 1u);
}
 
TEST(HospitalSystemTest, BookAppointmentFailsOnConflict) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_TRUE( sys.bookAppointment(docId, "2025-01-01", "10AM"));
    EXPECT_FALSE(sys.bookAppointment(docId, "2025-01-01", "10AM")); // conflict
}
 
TEST(HospitalSystemTest, BookAppointmentSetsCorrectPatientAndDoctorNames) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts[0].get_PatientName(), "Alice");
    EXPECT_EQ(appts[0].get_DoctorName(),  "Dr. Smith");
}
 
TEST(HospitalSystemTest, BookAppointmentSetsStatusToScheduled) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Scheduled");
}
 
TEST(HospitalSystemTest, BookAppointmentIncrementsAppointmentId) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
    auto appts = sys.adminViewAllAppointments();
    EXPECT_EQ(appts[0].get_AppointmentId(), 1);
    EXPECT_EQ(appts[1].get_AppointmentId(), 2);
}
 
// ─────────────────────────────────────────────────────────────
//  CANCEL APPOINTMENT (PATIENT)
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, CancelAppointmentFailsWhenNotLoggedIn) {
    HospitalSystem sys;
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAppointmentFailsWhenLoggedInAsAdmin) {
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAppointmentFailsWhenLoggedInAsDoctor) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAppointmentFailsForNonExistentAppointment) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(999));
}
 
TEST(HospitalSystemTest, CancelAppointmentSucceedsForOwnScheduledAppointment) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    EXPECT_TRUE(sys.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAppointmentChangesStatusToCancelled) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Cancelled");
}
 
TEST(HospitalSystemTest, CancelAppointmentFailsIfAlreadyCancelled) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    EXPECT_FALSE(sys.cancelAppointmentPatient(1)); // already cancelled
}
 
TEST(HospitalSystemTest, CancelAppointmentFailsForAnotherPatientsAppointment) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    // Alice books
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    // Bob tries to cancel Alice's appointment
    sys.login("bob@mail.com", "default");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));
}
 
// ─────────────────────────────────────────────────────────────
//  VIEW MY APPOINTMENTS
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, ViewMyAppointmentsReturnsEmptyWhenNotLoggedIn) {
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsReturnsEmptyForAdmin) {
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsReturnsEmptyForDoctor) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.viewMyAppointments().empty());
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsReturnsOnlyOwnAppointments) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    int docId = sys.adminViewAllUsers()[1]->get_id();
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
 
    sys.login("bob@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
 
    // Alice sees only her own
    sys.login("alice@mail.com", "default");
    auto appts = sys.viewMyAppointments();
    ASSERT_EQ(appts.size(), 1u);
    EXPECT_EQ(appts[0].get_PatientName(), "Alice");
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsIncludesCancelledAppointments) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.cancelAppointmentPatient(1);
    // Cancelled appointments are still visible to the patient
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
}
 
// ─────────────────────────────────────────────────────────────
//  VIEW DOCTOR SCHEDULE
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, ViewDoctorScheduleReturnsEmptyWhenNotLoggedIn) {
    HospitalSystem sys;
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleReturnsEmptyForPatient) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleReturnsEmptyForAdmin) {
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleReturnsOnlyOwnAppointments) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewDoctor("Dr. Jones", "jones@mail.com", "Neurology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
 
    auto users = sys.adminViewAllUsers();
    int smithId = users[1]->get_id();
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(smithId, "2025-01-01", "10AM");
 
    sys.login("smith@mail.com", "default");
    auto schedule = sys.viewDoctorSchedule();
    ASSERT_EQ(schedule.size(), 1u);
    EXPECT_EQ(schedule[0].get_DoctorName(), "Dr. Smith");
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleShowsNoAppointmentsForDoctorWithNone) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());
}
 
// ─────────────────────────────────────────────────────────────
//  COMPLETE APPOINTMENT (DOCTOR)
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, CompleteAppointmentFailsWhenNotLoggedIn) {
    HospitalSystem sys;
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAppointmentFailsWhenLoggedInAsPatient) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAppointmentFailsWhenLoggedInAsAdmin) {
    HospitalSystem sys;
    sys.login("admin@mail.com", "admin123");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAppointmentFailsForNonExistentAppointment) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.login("smith@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(999));
}
 
TEST(HospitalSystemTest, CompleteAppointmentSucceedsForOwnScheduledAppointment) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    EXPECT_TRUE(sys.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAppointmentChangesStatusToCompleted) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    sys.completeAppointmentDoctor(1);
    EXPECT_EQ(sys.adminViewAllAppointments()[0].get_Status(), "Completed");
}
 
TEST(HospitalSystemTest, CompleteAppointmentFailsIfAlreadyCompleted) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("smith@mail.com", "default");
    sys.completeAppointmentDoctor(1);
    EXPECT_FALSE(sys.completeAppointmentDoctor(1)); // already completed
}
 
TEST(HospitalSystemTest, CompleteAppointmentFailsForAnotherDoctorsAppointment) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewDoctor("Dr. Jones", "jones@mail.com", "Neurology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    int smithId = sys.adminViewAllUsers()[1]->get_id();
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(smithId, "2025-01-01", "10AM");
    // Jones tries to complete Smith's appointment
    sys.login("jones@mail.com", "default");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));
}
 
// ─────────────────────────────────────────────────────────────
//  ADMIN VIEW ALL APPOINTMENTS & USERS
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, AdminViewAllAppointmentsReturnsAll) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.registerNewPatient("Bob",   "bob@mail.com",   "222");
    int docId = sys.adminViewAllUsers()[1]->get_id();
 
    sys.login("alice@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "10AM");
    sys.login("bob@mail.com", "default");
    sys.bookAppointment(docId, "2025-01-01", "11AM");
 
    EXPECT_EQ(sys.adminViewAllAppointments().size(), 2u);
}
 
TEST(HospitalSystemTest, AdminViewAllUsersReturnsEveryRegisteredUser) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    // admin + doctor + patient = 3
    EXPECT_EQ(sys.adminViewAllUsers().size(), 3u);
}
 
// ─────────────────────────────────────────────────────────────
//  FIND DOCTOR BY ID (exercised indirectly through bookAppointment)
// ─────────────────────────────────────────────────────────────
 
TEST(HospitalSystemTest, FindDoctorByIdFailsForPatientId) {
    HospitalSystem sys;
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int patId = sys.adminViewAllUsers()[1]->get_id();
    // Booking with a patient's ID as doctorId should fail (no doctor found)
    EXPECT_FALSE(sys.bookAppointment(patId, "2025-01-01", "10AM"));
}
 
TEST(HospitalSystemTest, FindDoctorByIdSucceedsWithCorrectDoctorId) {
    HospitalSystem sys;
    sys.registerNewDoctor("Dr. Smith", "smith@mail.com", "Cardiology");
    sys.registerNewPatient("Alice", "alice@mail.com", "111");
    sys.login("alice@mail.com", "default");
    int docId = sys.adminViewAllUsers()[1]->get_id();
    EXPECT_TRUE(sys.bookAppointment(docId, "2025-01-01", "10AM"));
}
 
// ─────────────────────────────────────────────────────────────
 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
