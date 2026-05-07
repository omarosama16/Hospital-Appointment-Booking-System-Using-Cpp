#pragma once

#include <string>

struct AppointmentInfo {
    std::string patientName;
    std::string doctorName;
    std::string date;
    std::string time;
    std::string status;
};

class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;

    AppointmentInfo info;

public:
    Appointment();
    Appointment(int appointmentId, int patientId, int doctorId, const AppointmentInfo& info);

    void cancel();
    void complete();

    void print_row() const;

    std::string get_Date() const;
    std::string get_Time() const;
    std::string get_Status() const;
};
