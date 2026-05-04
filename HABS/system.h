#ifndef HOSPITAL_SYSTEM_H
#define HOSPITAL_SYSTEM_H

#include "user.h"
#include "Appointment.h"
#include "Doctor.h"
#include <vector>

using namespace std;

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
    HospitalSystem();
    ~HospitalSystem();
    bool login(string e, string password);
    void registerNewPatient(string name, string email, string phone);
    bool checkConflict(int docId, string date, string time);

    bool bookAppointment(int docid, string date, string time);
    bool cancelAppointmentPatient(int appid);
    vector<Appointment> viewMyAppointments();
    vector<Appointment> viewDoctorSchedule();
    bool completeAppointmentDoctor(int apptId);

    vector<Appointment> adminViewAllAppointments();
    vector<User *> adminViewAllUsers();
    void registerNewDoctor(string name, string email, string specialization);
};

#endif
