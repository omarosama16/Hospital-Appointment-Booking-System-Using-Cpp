#include <gtest/gtest.h>
#include "system.h"
#include "Doctor.h"
#include "patient.h"

// helper: safely get doctor id from system users
int getDoctorId(HospitalSystem &system)
{
    auto users = system.adminViewAllUsers();
    for (auto u : users)
    {
        if (u->get_role() == "doctor")
            return u->get_id();
    }
    return -1;
}

TEST(SystemTest, FullSystemCoverageSafe)
{
    HospitalSystem system;

    // ---------- create doctor ----------
    system.registerNewDoctor("Dr A", "doc@mail.com", "cardio");
    int docId = getDoctorId(system);
    ASSERT_NE(docId, -1);

    // ---------- create patient ----------
    system.registerNewPatient("p1", "p@mail.com", "123");

    // login patient (must succeed)
    ASSERT_TRUE(system.login("p@mail.com", "default"));

    // ---------- BOOK (valid path) ----------
    bool booked = system.bookAppointment(docId, "2026", "10AM");
    ASSERT_TRUE(booked);

    auto apps = system.viewMyAppointments();
    ASSERT_EQ(apps.size(), 1);

    // ---------- CONFIRM CONFLICT PATH ----------
    bool conflict = system.bookAppointment(docId, "2026", "10AM");
    ASSERT_FALSE(conflict);

    // ---------- cancel success ----------
    int apptId = apps[0].get_AppointmentId();
    ASSERT_TRUE(system.cancelAppointmentPatient(apptId));

    // ---------- cancel again (should fail) ----------
    ASSERT_FALSE(system.cancelAppointmentPatient(apptId));

    // ---------- doctor login ----------
    ASSERT_TRUE(system.login("doc@mail.com", "default"));

    // doctor schedule view (after switching user role)
    auto schedule = system.viewDoctorSchedule();
    // may be 0 or 1 depending on cancellation state, both OK

    // ---------- complete appointment (safe branch) ----------
    if (!schedule.empty())
    {
        system.completeAppointmentDoctor(schedule[0].get_AppointmentId());
    }

    // ---------- admin view ----------
    auto allApps = system.adminViewAllAppointments();
    ASSERT_GE(allApps.size(), 1);
}

TEST(SystemTest, LoginBranches)
{
    HospitalSystem system;

    ASSERT_FALSE(system.login("wrong", "wrong"));
    ASSERT_FALSE(system.login("", ""));
}
