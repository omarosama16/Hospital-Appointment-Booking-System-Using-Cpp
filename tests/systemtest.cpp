#include <gtest/gtest.h>
#include "system.h"
#include "patient.h"
#include "Doctor.h"
#include "Appointment.h"

class HospitalSystemTest : public ::testing::Test {
protected:
    HospitalSystem sys;

    void SetUp() override {

        sys.registerNewDoctor("Dr. House", "house@hospital.com", "Diagnostics"); 
        sys.registerNewPatient("Omar", "omar@test.com", "010123");       
    }
};


TEST_F(HospitalSystemTest, AdminInitialLoginWorks) {
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
}

TEST_F(HospitalSystemTest, PatientLoginWithDefaultPassword) {
    EXPECT_TRUE(sys.login("omar@test.com", "default"));
}

TEST_F(HospitalSystemTest, LoginFailsWithWrongCredentials) {
    EXPECT_FALSE(sys.login("omar@test.com", "wrong_pass"));
    EXPECT_FALSE(sys.login("nonexistent@test.com", "default"));
}


TEST_F(HospitalSystemTest, SuccessfulAppointmentBooking) {
    sys.login("omar@test.com", "default");
    
    bool result = sys.bookAppointment(2, "2026-05-20", "10AM");
    
    EXPECT_TRUE(result);
    auto myAppts = sys.viewMyAppointments();
    ASSERT_EQ(myAppts.size(), 1);
    EXPECT_EQ(myAppts[0].get_Status(), "Scheduled");
    EXPECT_EQ(myAppts[0].get_Time(), "10AM");
}

TEST_F(HospitalSystemTest, DetectsSchedulingConflict) {
    sys.login("omar@test.com", "default");
    
    sys.bookAppointment(2, "2026-05-20", "11AM");
    
    bool conflict = sys.bookAppointment(2, "2026-05-20", "11AM");
    
    EXPECT_FALSE(conflict);
}


TEST_F(HospitalSystemTest, DoctorCanViewAndCompleteAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-22", "10AM");

    sys.login("house@hospital.com", "default");
    auto schedule = sys.viewDoctorSchedule();
    ASSERT_FALSE(schedule.empty());
    
    int apptId = schedule[0].get_AppointmentId();
    
    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
    
    auto updatedSchedule = sys.viewDoctorSchedule();
    EXPECT_EQ(updatedSchedule[0].get_Status(), "Completed");
}


TEST_F(HospitalSystemTest, PatientCanCancelAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-25", "10AM");
    
    auto appts = sys.viewMyAppointments();
    int idToCancel = appts[0].get_AppointmentId();
    
    EXPECT_TRUE(sys.cancelAppointmentPatient(idToCancel));
    
    auto updatedAppts = sys.viewMyAppointments();
    EXPECT_EQ(updatedAppts[0].get_Status(), "Cancelled");
}

TEST_F(HospitalSystemTest, RoleSecurityPreventsIllegalActions) {
    sys.login("house@hospital.com", "default");
    
    bool result = sys.bookAppointment(2, "2026-05-25", "10AM");
    
    EXPECT_FALSE(result);
}


TEST_F(HospitalSystemTest, AdminCanSeeAllUsers) {
    sys.login("admin@mail.com", "admin123");
    
    auto allUsers = sys.adminViewAllUsers();
    
    EXPECT_EQ(allUsers.size(), 3);
}

TEST_F(HospitalSystemTest, AdminCanSeeAllAppointments) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-06-01", "10AM");
    sys.bookAppointment(2, "2026-06-02", "11AM");
    
    sys.login("admin@mail.com", "admin123");
    auto allAppts = sys.adminViewAllAppointments();
    
    EXPECT_EQ(allAppts.size(), 2);
}
