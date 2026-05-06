#include "system.h"
#include "Doctor.h"
#include "Patient.h"
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

User *HospitalSystem::getCurrentUser() const
{
    return currentUser;
}

Doctor *HospitalSystem::findDoctorById(int id)
{
    for (auto u : allUsers)
    {
        if (u->get_role() == "doctor")
        {
            Doctor *d = dynamic_cast<Doctor *>(u);
            if (d && d->get_id() == id)
                return d;
        }
    }
    return nullptr;
}

bool HospitalSystem::login(std::string e, std::string p)
{
    for (auto u : allUsers)
    {
        if (u->Authenticate(e, p))
        {
            currentUser = u;
            return true;
        }
    }
    return false;
}

void HospitalSystem::registerNewPatient(std::string n, std::string e, std::string p, std::string phone)
{
    allUsers.push_back(new Patient(nextUserId++, n, e, p, phone));
}

void HospitalSystem::registerNewDoctor(std::string n, std::string e, std::string p, std::string s)
{
    Doctor *d = new Doctor(nextUserId++, n, e, p, s);
    d->addAvailability("10AM");
    d->addAvailability("11AM");
    allUsers.push_back(d);
}

bool HospitalSystem::bookAppointment(int docId, std::string date, std::string time)
{
    if (!currentUser || currentUser->get_role() != "patient")
        return false;

    Doctor *d = findDoctorById(docId);
    if (!d)
        return false;

    masterSchedule.push_back(Appointment(
        nextApptId++,
        currentUser->get_id(),
        docId,
        currentUser->get_name(),
        d->get_name(),
        date,
        time,
        "Scheduled"));

    return true;
}

bool HospitalSystem::cancelAppointmentPatient(int id)
{
    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == id &&
            a.get_PatientId() == currentUser->get_id())
        {
            a.cancel();
            return true;
        }
    }
    return false;
}

std::vector<Appointment> HospitalSystem::viewMyAppointments()
{
    std::vector<Appointment> res;

    for (auto &a : masterSchedule)
        if (a.get_PatientId() == currentUser->get_id())
            res.push_back(a);

    return res;
}

std::vector<Appointment> HospitalSystem::viewDoctorSchedule()
{
    std::vector<Appointment> res;

    for (auto &a : masterSchedule)
        if (a.get_DoctorId() == currentUser->get_id())
            res.push_back(a);

    return res;
}

bool HospitalSystem::completeAppointmentDoctor(int id)
{
    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == id &&
            a.get_DoctorId() == currentUser->get_id())
        {
            a.complete();
            return true;
        }
    }
    return false;
}

std::vector<Appointment> HospitalSystem::adminViewAllAppointments()
{
    return masterSchedule;
}

std::vector<User *> HospitalSystem::adminViewAllUsers()
{
    return allUsers;
}