#include "Appointment.h"
#include <iostream>
#include <iomanip>

using namespace std;

/* ================= Constructors ================= */

Appointment::Appointment()
    : appointmentId(0), patientId(0), doctorId(0)
{
    info.date = "";
    info.time = "";
    info.status = "";
    info.patientName = "";
    info.doctorName = "";
}

Appointment::Appointment(int aId, int pId, int dId, const AppointmentInfo& inf)
    : appointmentId(aId), patientId(pId), doctorId(dId), info(inf)
{}

/* ================= Actions ================= */

void Appointment::cancel()
{
    info.status = "Cancelled";
}

void Appointment::complete()
{
    if (info.status != "Cancelled")
        info.status = "Completed";
}

/* ================= Display ================= */

void Appointment::print_row() const
{
    cout << left
         << setw(5) << appointmentId
         << setw(20) << info.patientName
         << setw(20) << info.doctorName
         << setw(12) << info.date
         << setw(10) << info.time
         << setw(12) << info.status
         << "\n";
}

/* ================= Getters ================= */

string Appointment::get_Date() const { return info.date; }
string Appointment::get_Time() const { return info.time; }
string Appointment::get_Status() const { return info.status; }
