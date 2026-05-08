#include "system.h"
#include "Doctor.h"
#include "Patient.h"
#include "admin.h"
#include <memory>

HospitalSystem::HospitalSystem()
{
    nextUserId = 1;
    nextApptId = 1;
    currentUser = nullptr;

    allUsers.push_back(std::make_unique<Admin>(nextUserId++, "admin", "admin@mail.com", "admin123"));
}

// No destructor needed — unique_ptr cleans up automatically.

User *HospitalSystem::getCurrentUser() const
{
    return currentUser;
}

Doctor *HospitalSystem::findDoctorById(int id)
{
    // Fix: const auto & — loop only reads each unique_ptr, never modifies it
    for (const auto &u : allUsers)
    {
        if (u->get_role() == "doctor")
        {
            // Fix: pointer-to-const — only reading the Doctor to check its id
            const Doctor *d = dynamic_cast<const Doctor *>(u.get());

            if (d && d->get_id() == id)
                return const_cast<Doctor *>(d);
        }
    }

    return nullptr;
}

bool HospitalSystem::login(std::string e, std::string p)
{
    // Fix: const auto & — loop only reads each unique_ptr to authenticate
    for (const auto &u : allUsers)
    {
        if (u->Authenticate(e, p))
        {
            currentUser = u.get();
            return true;
        }
    }

    return false;
}

void HospitalSystem::registerNewPatient(std::string n,
                                        std::string e,
                                        std::string p,
                                        std::string phone)
{
    allUsers.push_back(std::make_unique<Patient>(nextUserId++, n, e, p, phone));
}

void HospitalSystem::registerNewDoctor(std::string n,
                                       std::string e,
                                       std::string p,
                                       std::string s)
{
    auto d = std::make_unique<Doctor>(nextUserId++, n, e, p, s);

    d->addAvailability("10AM");
    d->addAvailability("11AM");

    allUsers.push_back(std::move(d));
}

bool HospitalSystem::bookAppointment(int docId,
                                     std::string date,
                                     std::string time)
{
    if (!currentUser)
        return false;

    if (currentUser->get_role() != "patient")
        return false;

    // Fix: pointer-to-const — d is only read to get the doctor's name
    const Doctor *d = findDoctorById(docId);

    if (!d)
        return false;

    AppointmentInfo info{currentUser->get_name(), d->get_name(), date, time};
    masterSchedule.push_back(Appointment(
        nextApptId++,
        currentUser->get_id(),
        docId,
        info,
        Status::Scheduled));

    return true;
}

bool HospitalSystem::cancelAppointmentPatient(int id)
{
    if (!currentUser)
        return false;

    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == id &&
            a.get_PatientId() == currentUser->get_id())
        {
            if (a.get_Status() != Status::Scheduled)
                return false;

            a.cancel();   // mutates — auto & is correct here
            return true;
        }
    }

    return false;
}

std::vector<Appointment> HospitalSystem::viewMyAppointments()
{
    std::vector<Appointment> res;

    if (!currentUser)
        return res;

    for (const auto &a : masterSchedule)
    {
        if (a.get_PatientId() == currentUser->get_id())
            res.push_back(a);
    }

    return res;
}

std::vector<Appointment> HospitalSystem::viewDoctorSchedule()
{
    std::vector<Appointment> res;

    if (!currentUser)
        return res;

    for (const auto &a : masterSchedule)
    {
        if (a.get_DoctorId() == currentUser->get_id())
            res.push_back(a);
    }

    return res;
}

bool HospitalSystem::completeAppointmentDoctor(int id)
{
    if (!currentUser || currentUser->get_role() != "doctor")
        return false;

    for (auto &a : masterSchedule)
    {
        if (a.get_AppointmentId() == id &&
            a.get_DoctorId() == currentUser->get_id())
        {
            if (a.get_Status() != Status::Scheduled)
                return false;

            a.complete();  // mutates — auto & is correct here
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
    std::vector<User *> raw;
    raw.reserve(allUsers.size());

    // Fix: const auto & — loop only reads each unique_ptr to extract raw pointer
    for (const auto &u : allUsers)
        raw.push_back(u.get());

    return raw;
}
