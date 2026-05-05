#include <gtest/gtest.h>
#include "system.h"
 
#define DEFAULT_PASS "default"
#define ADMIN_EMAIL  "admin@mail.com"
#define ADMIN_PASS   "admin123"
 
 
TEST(HospitalSystemTest, LoginAdminSucceeds) {
    HospitalSystem hs;
    EXPECT_TRUE(hs.login(ADMIN_EMAIL, ADMIN_PASS));
}
 
TEST(HospitalSystemTest, LoginPatientSucceeds) {
    HospitalSystem hs;
    hs.registerNewPatient("Layla", "layla@test.com", "0501234567");
    EXPECT_TRUE(hs.login("layla@test.com", DEFAULT_PASS));
}
 
TEST(HospitalSystemTest, LoginDoctorSucceeds) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Hana", "hana@test.com", "Oncology");
    EXPECT_TRUE(hs.login("hana@test.com", DEFAULT_PASS));
}
 
TEST(HospitalSystemTest, LoginWrongPasswordFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Tarek", "tarek@test.com", "0509999999");
    EXPECT_FALSE(hs.login("tarek@test.com", "wrongpass"));
}
 
TEST(HospitalSystemTest, LoginUnregisteredEmailFails) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.login("nobody@test.com", "pass"));
}
 
 
TEST(HospitalSystemTest, RegisterPatientAppearsInUserList) {
    HospitalSystem hs;
    hs.registerNewPatient("Ali", "ali@test.com", "0500000001");
    auto users = hs.adminViewAllUsers();
    bool found = false;
    for (auto u : users)
        if (u->get_email() == "ali@test.com") found = true;
    EXPECT_TRUE(found);
}
 
TEST(HospitalSystemTest, RegisterMultiplePatientsAllAppear) {
    HospitalSystem hs;
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");
    EXPECT_EQ(hs.adminViewAllUsers().size(), 3);
}
 
 
TEST(HospitalSystemTest, RegisterDoctorAppearsInUserList) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Ziad", "ziad@test.com", "Neurology");
    auto users = hs.adminViewAllUsers();
    bool found = false;
    for (auto u : users)
        if (u->get_email() == "ziad@test.com") found = true;
    EXPECT_TRUE(found);
}
 
TEST(HospitalSystemTest, RegisterDoctorHasDefaultAvailability) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Reem", "reem@test.com", "Dermatology");
    EXPECT_TRUE(hs.login("reem@test.com", DEFAULT_PASS));
    auto schedule = hs.viewDoctorSchedule();
    EXPECT_TRUE(schedule.empty()); 
}
 
 
TEST(HospitalSystemTest, BookAppointmentWithoutLoginFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. X", "x@test.com", "Surgery");
    EXPECT_FALSE(hs.bookAppointment(2, "2025-10-01", "10:00"));
}
 
TEST(HospitalSystemTest, BookAppointmentAsAdminFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. X", "x@test.com", "Surgery");
    hs.login(ADMIN_EMAIL, ADMIN_PASS);
    EXPECT_FALSE(hs.bookAppointment(2, "2025-10-01", "10:00"));
}
 
TEST(HospitalSystemTest, BookAppointmentAsDoctorFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. A", "a@test.com", "Cardiology");
    hs.registerNewDoctor("Dr. B", "b@test.com", "Neurology");
    hs.login("a@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.bookAppointment(3, "2025-10-01", "10:00"));
}
 
TEST(HospitalSystemTest, BookAppointmentWithNonExistentDoctorFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Sara", "sara@test.com", "0500000003");
    hs.login("sara@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.bookAppointment(999, "2025-10-01", "10:00"));
}
 
TEST(HospitalSystemTest, BookAppointmentSucceeds) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");     
    hs.login("nour@test.com", DEFAULT_PASS);
    EXPECT_TRUE(hs.bookAppointment(2, "2025-11-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookAppointmentAppearsInPatientView) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology");
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");     
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    EXPECT_EQ(hs.viewMyAppointments().size(), 1);
}
 
TEST(HospitalSystemTest, BookAppointmentAppearsInAdminView) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");      
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    EXPECT_EQ(hs.adminViewAllAppointments().size(), 1);
}
 
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseWhenNoAppointments) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.checkConflict(1, "2025-11-01", "09:00"));
}
 
TEST(HospitalSystemTest, CheckConflictDetectsDuplicate) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology");
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");    
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    EXPECT_TRUE(hs.checkConflict(2, "2025-11-01", "10AM"));
}
 
TEST(HospitalSystemTest, BookingConflictingSlotFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");     
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");      
    hs.login("p1@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    hs.login("p2@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.bookAppointment(2, "2025-11-01", "10AM"));
}
 
 
TEST(HospitalSystemTest, CancelWithoutLoginFails) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAsAdminFails) {
    HospitalSystem hs;
    hs.login(ADMIN_EMAIL, ADMIN_PASS);
    EXPECT_FALSE(hs.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelNonExistentAppointmentFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Amr", "amr@test.com", "0522222222");
    hs.login("amr@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.cancelAppointmentPatient(999));
}
 
TEST(HospitalSystemTest, CancelOwnAppointmentSucceeds) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology");
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");      
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM"); 
    EXPECT_TRUE(hs.cancelAppointmentPatient(1));
}
 
TEST(HospitalSystemTest, CancelAnotherPatientsAppointmentFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");        
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");       
    hs.login("p1@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM"); 
    hs.login("p2@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.cancelAppointmentPatient(1));
}
 
 
TEST(HospitalSystemTest, ViewMyAppointmentsWithoutLoginReturnsEmpty) {
    HospitalSystem hs;
    EXPECT_TRUE(hs.viewMyAppointments().empty());
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsAsAdminReturnsEmpty) {
    HospitalSystem hs;
    hs.login(ADMIN_EMAIL, ADMIN_PASS);
    EXPECT_TRUE(hs.viewMyAppointments().empty());
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsReturnsOnlyOwnAppointments) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");      
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");      
    hs.login("p1@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    hs.login("p2@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-02", "11AM");
    EXPECT_EQ(hs.viewMyAppointments().size(), 1);
}
 
 
TEST(HospitalSystemTest, ViewDoctorScheduleWithoutLoginReturnsEmpty) {
    HospitalSystem hs;
    EXPECT_TRUE(hs.viewDoctorSchedule().empty());
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleAsPatientReturnsEmpty) {
    HospitalSystem hs;
    hs.registerNewPatient("Sara", "sara@test.com", "0500000003");
    hs.login("sara@test.com", DEFAULT_PASS);
    EXPECT_TRUE(hs.viewDoctorSchedule().empty());
}
 
TEST(HospitalSystemTest, ViewDoctorScheduleShowsBookedAppointment) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");     
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    hs.login("karim@test.com", DEFAULT_PASS);
    EXPECT_EQ(hs.viewDoctorSchedule().size(), 1);
}
 
 
TEST(HospitalSystemTest, CompleteWithoutLoginFails) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAsPatientFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Amr", "amr@test.com", "0522222222");
    hs.login("amr@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteNonExistentAppointmentFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology");
    hs.login("karim@test.com", DEFAULT_PASS);
    EXPECT_FALSE(hs.completeAppointmentDoctor(999));
}
 
TEST(HospitalSystemTest, CompleteOwnAppointmentSucceeds) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology");
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004");      
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    hs.login("karim@test.com", DEFAULT_PASS);
    EXPECT_TRUE(hs.completeAppointmentDoctor(1));
}
 
TEST(HospitalSystemTest, CompleteAnotherDoctorsAppointmentFails) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. A", "a@test.com", "Cardiology"); 
    hs.registerNewDoctor("Dr. B", "b@test.com", "Neurology"); 
    hs.registerNewPatient("Nour", "nour@test.com", "0500000004"); 
    hs.login("nour@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM"); 
    hs.login("b@test.com", DEFAULT_PASS);       
    EXPECT_FALSE(hs.completeAppointmentDoctor(1));
}
 
 
TEST(HospitalSystemTest, AdminViewAllAppointmentsInitiallyEmpty) {
    HospitalSystem hs;
    EXPECT_TRUE(hs.adminViewAllAppointments().empty());
}
 
TEST(HospitalSystemTest, AdminViewAllAppointmentsReturnsAll) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Karim", "karim@test.com", "Cardiology"); 
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");          
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");       
    hs.login("p1@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-01", "10AM");
    hs.login("p2@test.com", DEFAULT_PASS);
    hs.bookAppointment(2, "2025-11-02", "11AM");
    EXPECT_EQ(hs.adminViewAllAppointments().size(), 2);
}
