#pragma once

#include <vector>
#include <string>
#include <memory>
#include "user.h"
#include "Appointment.h"

class Doctor; // forward declaration

class HospitalSystem
{
public:
    HospitalSystem();
    // No destructor needed — unique_ptr manages memory automatically

    User *getCurrentUser() const;

    bool login(std::string e, std::string p);

    void registerNewPatient(std::string n, std::string e,
                            std::string p, std::string phone);

    void registerNewDoctor(std::string n, std::string e,
                           std::string p, std::string s);

    bool bookAppointment(int docId, std::string date, std::string time);
    bool cancelAppointmentPatient(int id);
    bool completeAppointmentDoctor(int id);

    std::vector<Appointment> viewMyAppointments();
    std::vector<Appointment> viewDoctorSchedule();
    std::vector<Appointment> adminViewAllAppointments();

    // Returns non-owning raw pointers — callers must not delete them
    std::vector<User *> adminViewAllUsers();

private:
    // unique_ptr gives automatic memory management; no manual delete needed
    std::vector<std::unique_ptr<User>> allUsers;
    std::vector<Appointment>           masterSchedule;

    User *currentUser;
    int   nextUserId;
    int   nextApptId;

    Doctor *findDoctorById(int id);
};
