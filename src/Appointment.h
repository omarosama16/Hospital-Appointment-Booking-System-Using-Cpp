#include <iostream>
using namespace std;
#ifndef APPOINTMENT_H
#define APPOINTMENT_H

class Appointment
{
private:
    int appointmentid;
    int patientid;
    int doctorid;
    string patientname;
    string doctorname;
    string date;
    string time;
    string status;

public:
    Appointment();
    Appointment(int a, int p, int d, string pa, string doc, string da, string t, string s);
    void print_row() const;
    void cancel();
    void complete();
    int get_AppointmentId() const;
    int get_PatientId() const;
    int get_DoctorId() const;
    string get_Date() const;
    string get_Time() const;
    string get_Status() const;
};
#endif
