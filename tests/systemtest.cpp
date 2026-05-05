#include <gtest/gtest.h>
#include "system.h"
#include "patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "admin.h"

class TestPatient : public patient {
public:
    using patient::patient;
    string get_role() override { return "patient"; }
};

class TestDoctor : public Doctor {
public:
    using Doctor::Doctor;
    string get_role() override { return "doctor"; } 
};

class HospitalSystemTest : public ::testing::Test {
protected:
    HospitalSystem sys;
    int docId;
    int patId;

    void SetUp() override {

        sys.registerNewDoctor("Dr. Smith", "smith@test.com", "General");
        sys.registerNewPatient("Omar", "omar@test.com", "010123");      
        
        docId = 2;
        patId = 3;
    }
};

TEST_F(HospitalSystemTest, LoginVerification) {
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
    EXPECT_TRUE(sys.login("omar@test.com", "default"));
}

TEST_F(HospitalSystemTest, SuccessfulBookingFlow) {
    sys.login("omar@test.com", "default");
    

    bool booked = sys.bookAppointment(docId, "2026-05-15", "10AM");
    
    if (!booked) {
        std::cout << "[DIAGNOSTIC] Booking failed. Checking role strings..." << std::endl;
    }

    ASSERT_TRUE(booked) << "Role mismatch: System expects 'patient' (lowercase).";

    auto myAppts = sys.viewMyAppointments();
    ASSERT_EQ(myAppts.size(), 1);
    EXPECT_EQ(myAppts[0].get_Status(), "Scheduled");
}

TEST_F(HospitalSystemTest, DoctorCanCompleteAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(docId, "2026-05-20", "09AM");

    sys.login("smith@test.com", "default");
    
    auto schedule = sys.viewDoctorSchedule();
    

    ASSERT_FALSE(schedule.empty()) << "Doctor role mismatch: System expects 'doctor' (lowercase).";
    
    int apptId = schedule[0].get_AppointmentId();
    EXPECT_TRUE(sys.completeAppointmentDoctor(apptId));
}

TEST_F(HospitalSystemTest, PatientCanCancelAppointment) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(docId, "2026-05-22", "01PM");

    auto appts = sys.viewMyAppointments();
    ASSERT_FALSE(appts.empty());
    
    int idToCancel = appts[0].get_AppointmentId();
    EXPECT_TRUE(sys.cancelAppointmentPatient(idToCancel));

    auto updatedAppts = sys.viewMyAppointments();
    EXPECT_EQ(updatedAppts[0].get_Status(), "Cancelled");
}

TEST_F(HospitalSystemTest, AdminViewIntegrity) {
    sys.login("omar@test.com", "default");
    sys.bookAppointment(docId, "2026-10-10", "10AM");

    sys.login("admin@mail.com", "admin123");
    auto allAppts = sys.adminViewAllAppointments();

    EXPECT_GE(allAppts.size(), 1);
}
