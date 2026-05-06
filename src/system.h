#ifndef SYSTEM_H
#define SYSTEM_H

#include "user.h"
#include "Appointment.h"
#include "Doctor.h"
#include <vector>

class HospitalSystem
{
private:
    vector<User *> allUsers;
    vector<Appointment> masterSchedule;
    User *currentUser;

    int nextUserId;
    int nextApptId;

    Doctor *findDoctorById(int docId);

public:
    User *getCurrentUser() const;
    HospitalSystem();
    ~HospitalSystem();

    bool login(string e, string password);

    void registerNewPatient(string name, string email, string password, string phone);
    void registerNewDoctor(string name, string email, string password, string specialization);

    bool bookAppointment(int docid, string date, string time);
    bool cancelAppointmentPatient(int appid);

    vector<Appointment> viewMyAppointments();
    vector<Appointment> viewDoctorSchedule();

    bool completeAppointmentDoctor(int apptId);

    vector<Appointment> adminViewAllAppointments();
    vector<User *> adminViewAllUsers();
};

#endif