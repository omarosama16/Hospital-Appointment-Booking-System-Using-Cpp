#pragma once

#include <string>
#include <string_view>

enum class Status { Scheduled, Cancelled, Completed };

// Groups the name and scheduling strings so the primary constructor stays under 7 params
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

    // Primary constructor — 5 params, satisfies the SonarCloud rule
    Appointment(int a, int p, int d, const AppointmentInfo &info, Status s);

    // Convenience constructor for existing call sites (e.g. tests)
    // Delegates to the primary constructor via AppointmentInfo
    Appointment(int a, int p, int d,
                std::string_view pa, std::string_view doc,
                std::string_view da, std::string_view t,
                Status s)
        : Appointment(a, p, d, AppointmentInfo{std::string(pa), std::string(doc),
                                               std::string(da), std::string(t)}, s) {}

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
