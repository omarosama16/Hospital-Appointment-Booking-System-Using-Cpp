#include <gtest/gtest.h>
#include "system.h"

/* ================= TEST ACCESSOR ================= */
// Used to access protected/private admin methods for verification
class SysTest : public HospitalSystem {
public:
    using HospitalSystem::adminViewAllAppointments;
    using HospitalSystem::adminViewAllUsers;
};

/* ================= HELPERS ================= */
static void setupEnvironment(HospitalSystem &sys) {
    sys.registerNewDoctor("Dr. Smith", "d@m.com", "1", "General"); // ID 2
    sys.registerNewPatient("John", "p@m.com", "1", "555");         // ID 3
}

/* ================= 1. LOGIN & AUTHENTICATION ================= */

TEST(Coverage, Login_Branches) {
    HospitalSystem sys;
    // Branch: No user matches (find_if returns end())
    EXPECT_FALSE(sys.login("nonexistent@mail.com", "wrong"));
    
    // Branch: User exists but wrong password
    sys.registerNewPatient("User", "test@mail.com", "correct", "123");
    EXPECT_FALSE(sys.login("test@mail.com", "wrong_password"));
    
    // Branch: Success
    EXPECT_TRUE(sys.login("test@mail.com", "correct"));
    EXPECT_NE(sys.getCurrentUser(), nullptr);
}

/* ================= 2. DOCTOR SEARCH (findDoctorById) ================= */

TEST(Coverage, FindDoctor_Branches) {
    HospitalSystem sys;
    setupEnvironment(sys);
    sys.login("p@m.com", "1");

    // Branch: ID exists but is a Patient (u->get_role() != "doctor")
    EXPECT_FALSE(sys.bookAppointment(3, "2026-01-01", "10AM"));

    // Branch: ID exists but is an Admin (u->get_role() != "doctor")
    EXPECT_FALSE(sys.bookAppointment(1, "2026-01-01", "10AM"));

    // Branch: ID does not exist at all (Loop completes, returns nullptr)
    EXPECT_FALSE(sys.bookAppointment(999, "2026-01-01", "10AM"));
    
    // Branch: Success (u->get_role() == "doctor" and ID matches)
    EXPECT_TRUE(sys.bookAppointment(2, "2026-01-01", "10AM"));
}

/* ================= 3. APPOINTMENT BOOKING ================= */

TEST(Coverage, Booking_Security) {
    HospitalSystem sys;
    setupEnvironment(sys);

    // Branch: No currentUser logged in
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));

    // Branch: User logged in but is NOT a patient (e.g., Doctor trying to book)
    sys.login("d@m.com", "1");
    EXPECT_FALSE(sys.bookAppointment(2, "2026", "10AM"));
}

/* ================= 4. CANCELLATION & COMPLETION ================= */

TEST(Coverage, Appointment_Status_Transitions) {
    HospitalSystem sys;
    setupEnvironment(sys);
    
    // Setup: Book an appointment
    sys.login("p@m.com", "1");
    sys.bookAppointment(2, "2026", "10AM"); // Appt ID 1
    
    // Branch: cancelAppointment - Wrong user (Doctor trying to cancel patient's appt)
    sys.login("d@m.com", "1");
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));

    // Branch: cancelAppointment - Success
    sys.login("p@m.com", "1");
    EXPECT_TRUE(sys.cancelAppointmentPatient(1));

    // Branch: cancelAppointment - Already cancelled (Status != Scheduled)
    EXPECT_FALSE(sys.cancelAppointmentPatient(1));

    // Branch: completeAppointment - Wrong role
    sys.login("p@m.com", "1");
    EXPECT_FALSE(sys.completeAppointmentDoctor(1));

    // Branch: completeAppointment - Success
    // (Need a fresh scheduled appointment)
    sys.bookAppointment(2, "2026", "11AM"); // Appt ID 2
    sys.login("d@m.com", "1");
    EXPECT_TRUE(sys.completeAppointmentDoctor(2));

    // Branch: completeAppointment - Already completed (Status != Scheduled)
    EXPECT_FALSE(sys.completeAppointmentDoctor(2));
}

/* ================= 5. VIEWING LISTS ================= */

TEST(Coverage, ViewLists_EmptyBranches) {
    HospitalSystem sys;
    // Branch: !currentUser (Return empty vector)
    EXPECT_TRUE(sys.viewMyAppointments().empty());
    EXPECT_TRUE(sys.viewDoctorSchedule().empty());

    setupEnvironment(sys);
    
    // Branch: Patient views (Filters only their IDs)
    sys.login("p@m.com", "1");
    sys.bookAppointment(2, "2026", "10AM");
    EXPECT_EQ(sys.viewMyAppointments().size(), 1u);
    
    // Branch: Doctor views (Filters only their IDs)
    sys.login("d@m.com", "1");
    EXPECT_EQ(sys.viewDoctorSchedule().size(), 1u);
}

/* ================= 6. ADMIN & UTILITY ================= */

TEST(Coverage, Admin_And_Constructor) {
    SysTest sys;
    // Covers Constructor initialization
    EXPECT_EQ(sys.getCurrentUser(), nullptr);
    
    // Branch: adminViewAllUsers (Covering loop and reserve)
    auto users = sys.adminViewAllUsers();
    EXPECT_GE(users.size(), 1u); // Should have at least the default admin
    
    // Branch: find_if on empty/small list
    EXPECT_TRUE(sys.login("admin@mail.com", "admin123"));
}
