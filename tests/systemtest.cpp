#include <gtest/gtest.h>
#include "system.h"
 
TEST(HospitalSystemTest, RegisterAndLoginPatient) {
    HospitalSystem hs;
    hs.registerNewPatient("Layla", "layla@test.com", "0501234567");
    EXPECT_TRUE(hs.login("layla@test.com", "layla@test.com")); // adjust password logic to match your impl
}
 
TEST(HospitalSystemTest, LoginWithWrongPasswordFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Tarek", "tarek@test.com", "0509999999");
    EXPECT_FALSE(hs.login("tarek@test.com", "wrongpass"));
}
 
TEST(HospitalSystemTest, LoginWithUnregisteredEmailFails) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.login("nobody@test.com", "pass"));
}
 
TEST(HospitalSystemTest, RegisterNewDoctor) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Hana", "hana@test.com", "Oncology");
    EXPECT_TRUE(hs.login("hana@test.com", "hana@test.com")); // adjust to match your default password logic
}
 
TEST(HospitalSystemTest, AdminViewAllUsersReturnsRegisteredUsers) {
    HospitalSystem hs;
    hs.registerNewPatient("P1", "p1@test.com", "0500000001");
    hs.registerNewPatient("P2", "p2@test.com", "0500000002");
    hs.registerNewDoctor("D1", "d1@test.com", "Cardiology");
    auto users = hs.adminViewAllUsers();
    EXPECT_GE(users.size(), 3);
}
 
TEST(HospitalSystemTest, BookAppointmentRequiresLogin) {
    HospitalSystem hs;
    hs.registerNewDoctor("Dr. Ziad", "ziad@test.com", "Neurology");
    // Without login, booking should fail
    EXPECT_FALSE(hs.bookAppointment(1, "2025-10-01", "10:00"));
}
 
TEST(HospitalSystemTest, CheckConflictReturnsFalseWhenNoAppointments) {
    HospitalSystem hs;
    EXPECT_FALSE(hs.checkConflict(1, "2025-11-01", "09:00"));
}
 
TEST(HospitalSystemTest, AdminViewAllAppointmentsInitiallyEmpty) {
    HospitalSystem hs;
    auto appts = hs.adminViewAllAppointments();
    EXPECT_TRUE(appts.empty());
}
 
TEST(HospitalSystemTest, CancelNonExistentAppointmentFails) {
    HospitalSystem hs;
    hs.registerNewPatient("Reem", "reem@test.com", "0511111111");
    hs.login("reem@test.com", "reem@test.com");
    EXPECT_FALSE(hs.cancelAppointmentPatient(999));
}
 
TEST(HospitalSystemTest, ViewMyAppointmentsInitiallyEmpty) {
    HospitalSystem hs;
    hs.registerNewPatient("Amr", "amr@test.com", "0522222222");
    hs.login("amr@test.com", "amr@test.com");
    auto appts = hs.viewMyAppointments();
    EXPECT_TRUE(appts.empty());
}
 
