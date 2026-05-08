#pragma once

#include <string>
#include <string_view>

enum class Status { Scheduled, Cancelled, Completed };

// Groups the name and scheduling strings so the constructor stays under 7 params
struct AppointmentInfo
{
    std::string patientName;
    std::string doctorName;
    std::string date;
    std::string time;
};

class Appointment
{
public:
    Appointment();
    // 5 parameters: appointmentId, patientId, doctorId, info, status
    Appointment(int a, int p, int d, const AppointmentInfo &info, Status s);

    void cancel();
    void complete();
    void print_row() const;

    int         get_AppointmentId() const;
    int         get_PatientId()     const;
    int         get_DoctorId()      const;
    std::string get_Date()          const;
    std::string get_Time()          const;
    Status      get_Status()        const;

private:
    int         appointmentid;
    int         patientid;
    int         doctorid;
    std::string patientname;
    std::string doctorname;
    std::string date;
    std::string time;
    Status      status;
};
