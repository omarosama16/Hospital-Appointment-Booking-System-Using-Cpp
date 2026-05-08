#include "Appointment.h"
#include <iostream>

Appointment::Appointment()
    : appointmentid(0),
      patientid(0),
      doctorid(0),
      patientname(""),
      doctorname(""),
      date(""),
      time(""),
      status(Status::Scheduled)
{
}

Appointment::Appointment(int a, int p, int d, const AppointmentInfo &info, Status s)
    : appointmentid(a),
      patientid(p),
      doctorid(d),
      patientname(info.patientName),
      doctorname(info.doctorName),
      date(info.date),
      time(info.time),
      status(s)
{
}

void Appointment::cancel()
{
    if (status == Status::Scheduled)
        status = Status::Cancelled;
}

void Appointment::complete()
{
    if (status == Status::Scheduled)
        status = Status::Completed;
}

void Appointment::print_row() const
{
    std::string s;

    if (status == Status::Scheduled)
        s = "Scheduled";
    else if (status == Status::Cancelled)
        s = "Cancelled";
    else
        s = "Completed";

    std::cout << appointmentid << " "
              << patientname   << " "
              << doctorname    << " "
              << date          << " "
              << time          << " "
              << s             << "\n";
}

int         Appointment::get_AppointmentId() const { return appointmentid; }
int         Appointment::get_PatientId()     const { return patientid; }
int         Appointment::get_DoctorId()      const { return doctorid; }
std::string Appointment::get_Date()          const { return date; }
std::string Appointment::get_Time()          const { return time; }
Status      Appointment::get_Status()        const { return status; }
