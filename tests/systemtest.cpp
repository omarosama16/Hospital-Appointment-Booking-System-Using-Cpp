#include <gtest/gtest.h>
#include "system.h"
#include "patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "admin.h"

class HospitalSystemTest : public ::testing::Test {
protected:
    HospitalSystem sys;

    void SetUp() override {

        sys.registerNewDoctor("Dr. Smith", "smith@test.com", "General"); 
        sys.registerNewPatient("Omar", "omar@test.com", "010123");       
    }
};


TEST_F(HospitalSystemTest, AdminLoginVerification) {
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
}

TEST_F(HospitalSystemTest, NewUserDefaultPassword) {
    EXPECT_TRUE(sys.login("omar@test.com", "default"));
    EXPECT_TRUE(sys.login("smith@test.com", "default"));
}


TEST_F(HospitalSystemTest, FullBookingFlow) {
    sys.login("omar@test.com", "default");
    
    bool booked = sys.bookAppointment(2, "2026-05-15", "10AM");
    EXPECT_TRUE(booked);

    auto appointments = sys.viewMyAppointments();
    
    ASSERT_EQ(appointments.size(), 1) << "Appointment vector empty - booking failed.";
    EXPECT_EQ(appointments[0].get_Status(), "Scheduled");
    EXPECT_EQ(appointments[0].get_DoctorId(), 2);
}

TEST_F(HospitalSystemTest, PreventsDuplicateAppointments) {
    sys.login("omar@test.com", "default");
    
    sys.bookAppointment(2, "2026-05-15", "11AM");
    
    bool duplicate = sys.bookAppointment(2, "2026-05-15", "11AM");
    EXPECT_FALSE(duplicate);
}


TEST_F(HospitalSystemTest, DoctorCanCompleteAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-20", "09AM");

    sys.login("smith@test.com", "default");
    auto schedule = sys.viewDoctorSchedule();
    
    ASSERT_FALSE(schedule.empty()) << "Doctor schedule empty - booking did not link to Doctor.";
    int apptId = schedule[0].get_AppointmentId();

    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
    
    auto updatedSchedule = sys.viewDoctorSchedule();
    EXPECT_EQ(updatedSchedule[0].get_Status(), "Completed");
}


TEST_F(HospitalSystemTest, PatientCanCancelActiveAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-22", "01PM");

    auto appts = sys.viewMyAppointments();
    ASSERT_FALSE(appts.empty());
    
    int idToCancel = appts[0].get_AppointmentId();
    EXPECT_TRUE(sys.cancelAppointmentPatient(idToCancel));

    auto updatedAppts = sys.viewMyAppointments();
    EXPECT_EQ(updatedAppts[0].get_Status(), "Cancelled");
}

TEST_F(HospitalSystemTest, UnauthorizedRoleCannotBook) {
    sys.login("smith@test.com", "default");
    
    bool result = sys.bookAppointment(2, "2026-05-25", "10AM");
    EXPECT_FALSE(result);
}


TEST_F(HospitalSystemTest, SystemMaintainsAllUsers) {
    sys.login("admin@mail.com", "admin123");
    auto users = sys.adminViewAllUsers();
    
    EXPECT_EQ(users.size(), 3);
}

TEST_F(HospitalSystemTest, AppointmentPersistenceInMasterSchedule) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-10-10", "10AM");

    sys.login("admin@mail.com", "admin123");
    auto master = sys.adminViewAllAppointments();
    
    EXPECT_EQ(master.size(), 1);
}
