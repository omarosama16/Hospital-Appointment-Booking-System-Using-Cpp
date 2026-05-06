#ifndef SYSTEM_H
#define SYSTEM_H

#include "user.h"
#include "Appointment.h"
#include "Doctor.h"
#include <vector>

class HospitalSystem
{
private:
    std::vector<User *> allUsers;
    std::vector<Appointment> masterSchedule;
    User *currentUser;

    int nextUserId;
    int nextApptId;

    Doctor *findDoctorById(int docId);

public:
    HospitalSystem();
    ~HospitalSystem();

    User *getCurrentUser() const;

    bool login(std::string e, std::string password);

    void registerNewPatient(std::string name, std::string email, std::string password, std::string phone);
    void registerNewDoctor(std::string name, std::string email, std::string password, std::string specialization);

    bool bookAppointment(int docid, std::string date, std::string time);
    bool cancelAppointmentPatient(int appid);

    std::vector<Appointment> viewMyAppointments();
    std::vector<Appointment> viewDoctorSchedule();

    bool completeAppointmentDoctor(int apptId);

    std::vector<Appointment> adminViewAllAppointments();
    std::vector<User *> adminViewAllUsers();
};

#endif