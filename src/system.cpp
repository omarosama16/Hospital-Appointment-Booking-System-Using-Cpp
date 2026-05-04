#include <iostream>
using namespace std;
#include "system.h"
#include "user.h"
#include "Doctor.h"
#include "patient.h"
#include "admin.h"

HospitalSystem::HospitalSystem()
{
    nextUserId = 1;
    nextApptId = 1;
    currentUser = nullptr;

    allUsers.push_back(new Admin(nextUserId++, "admin", "admin@mail.com", "admin123"));
}

HospitalSystem::~HospitalSystem()
{
    for (auto u : allUsers)
        delete u;
}

Doctor *HospitalSystem::findDoctorById(int docId)
{
    for (auto u : allUsers)
    {
        if (u->get_role() == "doctor" && u->get_id() == docId)
            return (Doctor *)u;
    }
    return nullptr;
}
// 1
bool HospitalSystem::login(string email, string password)
{
    for (auto u : allUsers)
    {
        if (u->Authenticate(email, password))
        {
            currentUser = u;
            return true;
        }
    }
    return false;
}
// 2
void HospitalSystem::registerNewPatient(string name, string email, string phone)
{
    patient *p = new patient(nextUserId++, name, email, "default", phone);
    allUsers.push_back(p);
}

void HospitalSystem::registerNewDoctor(string name, string email, string specialization)
{
    Doctor *d = new Doctor(nextUserId++, name, email, "default", specialization);

    // default availability
    d->addAvailability("10AM");
    d->addAvailability("11AM");

    allUsers.push_back(d);
}

bool HospitalSystem::checkConflict(int docId, string date, string time)
{
    for (auto &a : masterSchedule)
    {
        if (a.get_DoctorId() == docId &&
            a.get_Date() == date &&
            a.get_Time() == time &&
            a.get_Status() == "Scheduled")
            return true;
    }
    return false;
}

bool HospitalSystem::bookAppointment(int docId, string date, string time)
{
    if (!currentUser || currentUser->get_role() != "patient")
        return false;

    if (checkConflict(docId, date, time))
        return false;

    Doctor *d = findDoctorById(docId);
    if (!d)
        return false;

    Appointment a(nextApptId++, currentUser->get_id(), d->get_id(),
                  currentUser->get_name(), d->get_name(), date, time, "Scheduled");

    masterSchedule.push_back(a);
    return true;
}

bool HospitalSystem::cancelAppointmentPatient(int apptId)
{
    if (!currentUser || currentUser->get_role() != "patient")
        return false;

    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == apptId &&
            a.get_PatientId() == currentUser->get_id() &&
            a.get_Status() == "Scheduled")
        {

            a.cancel();
            return true;
        }
    }
    return false;
}

vector<Appointment> HospitalSystem::viewMyAppointments()
{
    vector<Appointment> result;

    if (!currentUser || currentUser->get_role() != "patient")
        return result;

    for (auto &a : masterSchedule)
    {
        if (a.get_PatientId() == currentUser->get_id())
            result.push_back(a);
    }

    return result;
}

vector<Appointment> HospitalSystem::viewDoctorSchedule()
{
    vector<Appointment> result;

    if (!currentUser || currentUser->get_role() != "doctor")
        return result;

    for (auto &a : masterSchedule)
    {
        if (a.get_DoctorId() == currentUser->get_id())
            result.push_back(a);
    }

    return result;
}

bool HospitalSystem::completeAppointmentDoctor(int apptId)
{
    if (!currentUser || currentUser->get_role() != "doctor")
        return false;

    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == apptId &&
            a.get_DoctorId() == currentUser->get_id() &&
            a.get_Status() == "Scheduled")
        {

            a.complete();
            return true;
        }
    }
    return false;
}

vector<Appointment> HospitalSystem::adminViewAllAppointments()
{
    return masterSchedule;
}

vector<User *> HospitalSystem::adminViewAllUsers()
{
    return allUsers;
}
