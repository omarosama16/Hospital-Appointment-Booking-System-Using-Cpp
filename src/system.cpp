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

    // FIX Line 13: extracted assignment from expression
    const int adminId = nextUserId;
    nextUserId++;
    allUsers.push_back(std::make_unique<Admin>(adminId, "admin", "admin@mail.com", "admin123"));
}

User *HospitalSystem::getCurrentUser() const
{
    return currentUser;
}

Doctor *HospitalSystem::findDoctorById(int id)
{
    for (const auto &u : allUsers)
    {
        if (u->get_role() == "doctor")
        {
            auto *d = dynamic_cast<Doctor *>(u.get());
            if (d && d->get_id() == id)
                return d;
        }
    }
    return nullptr;
}

bool HospitalSystem::login(std::string_view e, std::string_view p)
{
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

void HospitalSystem::registerNewPatient(std::string_view n,
                                        std::string_view e,
                                        std::string_view p,
                                        std::string_view phone)
{
    // FIX: extracted assignment from expression
    const int patientId = nextUserId;
    nextUserId++;
    allUsers.push_back(std::make_unique<Patient>(patientId, n, e, p, phone));
}

void HospitalSystem::registerNewDoctor(std::string_view n,
                                       std::string_view e,
                                       std::string_view p,
                                       std::string_view s)
{
    // FIX: extracted assignment from expression
    const int doctorId = nextUserId;
    nextUserId++;
    auto d = std::make_unique<Doctor>(doctorId, n, e, p, s);
    d->addAvailability("10AM");
    d->addAvailability("11AM");
    allUsers.push_back(std::move(d));
}

bool HospitalSystem::bookAppointment(int docId,
                                     std::string_view date,
                                     std::string_view time)
{
    if (!currentUser)
        return false;
    if (currentUser->get_role() != "patient")
        return false;

    const Doctor *d = findDoctorById(docId);
    if (!d)
        return false;

    // FIX: extracted assignment from expression
    const int apptId = nextApptId;
    nextApptId++;
    AppointmentInfo info{currentUser->get_name(), d->get_name(),
                         std::string(date), std::string(time)};

    masterSchedule.emplace_back(apptId,
                                currentUser->get_id(),
                                docId,
                                info,
                                Status::Scheduled);
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
            a.cancel();
            return true;
        }
    }
    return false;
}

std::vector<Appointment> HospitalSystem::viewMyAppointments() const
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

std::vector<Appointment> HospitalSystem::viewDoctorSchedule() const
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
            a.complete();
            return true;
        }
    }
    return false;
}

std::vector<Appointment> HospitalSystem::adminViewAllAppointments() const
{
    return masterSchedule;
}

std::vector<User *> HospitalSystem::adminViewAllUsers() const
{
    std::vector<User *> raw;
    raw.reserve(allUsers.size());
    for (const auto &u : allUsers)
        raw.push_back(u.get());
    return raw;
}
