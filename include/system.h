#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include "user.h"
#include "Appointment.h"

class Doctor;

class HospitalSystem
{
public:
    HospitalSystem();
    // No destructor needed — unique_ptr manages memory automatically

    User *getCurrentUser() const;

    bool login(std::string_view e, std::string_view p);

    void registerNewPatient(std::string_view n, std::string_view e,
                            std::string_view p, std::string_view phone);

    void registerNewDoctor(std::string_view n, std::string_view e,
                           std::string_view p, std::string_view s);

    bool bookAppointment(int docId, std::string_view date, std::string_view time);
    bool cancelAppointmentPatient(int id);
    bool completeAppointmentDoctor(int id);

    std::vector<Appointment> viewMyAppointments();
    std::vector<Appointment> viewDoctorSchedule();
    std::vector<Appointment> adminViewAllAppointments();

    // Returns non-owning raw pointers — callers must not delete them
    std::vector<User *> adminViewAllUsers();

private:
    std::vector<std::unique_ptr<User>> allUsers;
    std::vector<Appointment>           masterSchedule;

    User *currentUser;
    int   nextUserId;
    int   nextApptId;

    Doctor *findDoctorById(int id);
};
