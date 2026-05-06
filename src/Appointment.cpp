#include "Appointment.h"
#include <iostream>
#include <iomanip>

using namespace std;

Appointment::Appointment()
{
    appointmentid = patientid = doctorid = 0;
    patientname = doctorname = date = time = status = "";
}

Appointment::Appointment(int a, int p, int d, string pa, string doc, string da, string t, string s)
    : appointmentid(a), patientid(p), doctorid(d),
      patientname(pa), doctorname(doc),
      date(da), time(t), status(s) {}

int Appointment::get_AppointmentId() const { return appointmentid; }
int Appointment::get_PatientId() const { return patientid; }
int Appointment::get_DoctorId() const { return doctorid; }
string Appointment::get_Date() const { return date; }
string Appointment::get_Time() const { return time; }
string Appointment::get_Status() const { return status; }

void Appointment::cancel()
{
    if (status == "Scheduled")
        status = "Cancelled";
}

void Appointment::complete()
{
    if (status == "Scheduled")
        status = "Completed";
}

void Appointment::print_row() const
{
    cout << left
         << setw(5) << appointmentid
         << setw(20) << patientname
         << setw(20) << doctorname
         << setw(12) << date
         << setw(10) << time
         << setw(12) << status << endl;
}