#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

class Appointment
{
private:
    int appointmentid;
    int patientid;
    int doctorid;
    std::string patientname;
    std::string doctorname;
    std::string date;
    std::string time;
    std::string status;

public:
    Appointment();
    Appointment(int a, int p, int d, std::string pa, std::string doc,
                std::string da, std::string t, std::string s);

    void cancel();
    void complete();
    void print_row() const;

    int get_AppointmentId() const;
    int get_PatientId() const;
    int get_DoctorId() const;

    std::string get_Date() const;
    std::string get_Time() const;
    std::string get_Status() const;
};

#endif