#include "Appointment.h"
#include <iostream>

Appointment::Appointment() : appointmentid(0), patientid(0), doctorid(0), date(""), time(""), status("") {}

Appointment::Appointment(int a, int p, int d, const AppointmentInfo& info)
{
    appointmentid = a;
    patientid     = p;
    doctorid      = d;
    patientname   = info.patientname;
    doctorname    = info.doctorname;
    date          = info.date;
    time          = info.time;
    status        = info.status;
}

void Appointment::cancel() { status = "Cancelled"; }
void Appointment::complete()
{
    if (status != "Cancelled")
    {
        status = "Completed";
    }
}

void Appointment::print_row() const
{
    std::cout << appointmentid << " "
              << patientname << " "
              << doctorname << " "
              << date << " "
              << time << " "
              << status << "\n";
}

int Appointment::get_AppointmentId() const { return appointmentid; }
int Appointment::get_PatientId()     const { return patientid; }
int Appointment::get_DoctorId()      const { return doctorid; }

std::string Appointment::get_Date()   const { return date; }
std::string Appointment::get_Time()   const { return time; }
std::string Appointment::get_Status() const { return status; }
