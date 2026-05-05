#include <gtest/gtest.h>
#include <algorithm> 
#include "system.h"
#include "patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "admin.h"

std::string toLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}

class HospitalSystemTest : public ::testing::Test {
protected:
    HospitalSystem sys;

    void SetUp() override {

        sys.registerNewDoctor("Dr. Smith", "smith@test.com", "General"); 
        sys.registerNewPatient("Omar", "omar@test.com", "010123");       
    }
};


TEST_F(HospitalSystemTest, LoginCheck) {
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_TRUE(sys.login("omar@test.com", "default"));
}


TEST_F(HospitalSystemTest, FullBookingFlow) {
    ASSERT_TRUE(sys.login("omar@test.com", "default"));
    

    bool booked = sys.bookAppointment(2, "2026-05-15", "10AM");
    
    if (!booked) {
        FAIL() << "Booking failed. Possible reasons: \n"
               << "1. currentUser role is 'Patient' but system checks for 'patient'\n"
               << "2. findDoctorById looking for 'doctor' but Doctor class has 'Doctor'";
    }

    auto appointments = sys.viewMyAppointments();
    ASSERT_EQ(appointments.size(), 1);
    EXPECT_EQ(appointments[0].get_Status(), "Scheduled");
}


TEST_F(HospitalSystemTest, DoctorScheduleFlow) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-05-20", "09AM");

    ASSERT_TRUE(sys.login("smith@test.com", "default"));
    
    auto schedule = sys.viewDoctorSchedule();
    
    ASSERT_FALSE(schedule.empty()) << "Doctor's schedule is empty. Check if Role is 'doctor' vs 'Doctor'";
    
    int apptId = schedule[0].get_AppointmentId();
    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
}


TEST_F(HospitalSystemTest, AdminViewAll) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(2, "2026-06-01", "10AM");

    sys.login("admin@mail.com", "admin123");
    
    auto allUsers = sys.adminViewAllUsers();
    auto allAppts = sys.adminViewAllAppointments();

    EXPECT_GE(allUsers.size(), 3); 
    EXPECT_GE(allAppts.size(), 1);
}


TEST_F(HospitalSystemTest, UnauthorizedAccess) {
    sys.login("admin@mail.com", "admin123");
    
    EXPECT_FALSE(sys.bookAppointment(2, "2026-06-01", "10AM"));
}
