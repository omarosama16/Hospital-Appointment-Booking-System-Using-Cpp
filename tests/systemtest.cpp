#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "system.h"
#include "patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "admin.h"

class HospitalSystemTest : public ::testing::Test {
protected:
    HospitalSystem sys;

    void SetUp() override {
        // IDs are assigned incrementally starting from 1 (Admin)
        // Admin: ID 1
        // Doctor: ID 2
        sys.registerNewDoctor("Dr. Smith", "smith@test.com", "General"); 
        // Patient: ID 3
        sys.registerNewPatient("Omar", "omar@test.com", "010123");       
    }
};

// --- AUTHENTICATION ---

TEST_F(HospitalSystemTest, LoginVerification) {
    // Test Admin login
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    
    // Test Patient login (using 'default' as set in registerNewPatient)
    EXPECT_TRUE(sys.login("omar@test.com", "default"));
}

// --- BOOKING LOGIC ---

TEST_F(HospitalSystemTest, SuccessfulBookingFlow) {
    // 1. Must be logged in as a patient to book
    sys.login("omar@test.com", "default");
    
    // 2. Book Doctor (ID 2)
    bool booked = sys.bookAppointment(2, "2026-05-15", "10AM");
    
    // If this fails, ensure Doctor role is "doctor" and Patient role is "patient"
    ASSERT_TRUE(booked) << "Booking failed! Check if role strings match ('patient' vs 'Patient')";

    auto myAppts = sys.viewMyAppointments();
    ASSERT_EQ(myAppts.size(), 1);
    EXPECT_EQ(myAppts[0].get_Status(), "Scheduled");
    EXPECT_EQ(myAppts[0].get_Time(), "10AM");
}

TEST_F(HospitalSystemTest, PreventsConflictBooking) {
    sys.login("omar@test.com", "default");
    
    // First booking
    sys.bookAppointment(2, "2026-05-15", "11AM");
    
    // Second booking same slot
    bool result = sys.bookAppointment(2, "2026-05-15", "11AM");
    EXPECT_FALSE(result); 
}

// --- DOCTOR ACTIONS ---

TEST_F(HospitalSystemTest, DoctorCanCompleteAppointment) {
    // Setup: Create an appointment
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-20", "09AM");

    // Action: Login as Doctor
    sys.login("smith@test.com", "default");
    auto schedule = sys.viewDoctorSchedule();
    
    // ASSERT prevents SegFault if vector is empty
    ASSERT_FALSE(schedule.empty()) << "Doctor schedule is empty. Ensure Doctor ID is 2 and role is 'doctor'";
    
    int apptId = schedule[0].get_AppointmentId();
    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
    
    auto updatedSchedule = sys.viewDoctorSchedule();
    EXPECT_EQ(updatedSchedule[0].get_Status(), "Completed");
}

// --- CANCELLATION ---

TEST_F(HospitalSystemTest, PatientCanCancelAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-22", "01PM");

    auto appts = sys.viewMyAppointments();
    ASSERT_FALSE(appts.empty());
    
    int idToCancel = appts[0].get_AppointmentId();
    EXPECT_TRUE(sys.cancelAppointmentPatient(idToCancel));

    auto updatedAppts = sys.viewMyAppointments();
    EXPECT_EQ(updatedAppts[0].get_Status(), "Cancelled");
}

// --- ADMIN VISIBILITY ---

TEST_F(HospitalSystemTest, AdminViewIntegrity) {
    // Setup data
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-10-10", "10AM");

    // Login as Admin
    sys.login("admin@mail.com", "admin123");
    
    auto allUsers = sys.adminViewAllUsers();
    auto allAppts = sys.adminViewAllAppointments();

    EXPECT_EQ(allUsers.size(), 3); // Admin, Smith, Omar
    EXPECT_EQ(allAppts.size(), 1);
}

// --- SECURITY ---

TEST_F(HospitalSystemTest, UnauthorizedRoleCannotBook) {
    // Admin (ID 1) logs in
    sys.login("admin@mail.com", "admin123");
    
    // Admin is not a patient, so booking should return false
    EXPECT_FALSE(sys.bookAppointment(2, "2026-06-01", "10AM"));
}
