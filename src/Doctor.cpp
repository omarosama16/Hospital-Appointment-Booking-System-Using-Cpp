#include "doctor.h"

Doctor::Doctor(int i, string n, string e, string p, string s)
    : User(i, n, e, p, "doctor"), specialization(s) {}

string Doctor::getSpecialization() const
{
    return specialization;
}

vector<string> Doctor::getAvailability() const
{
    return availability;
}

void Doctor::addAvailability(string time)
{
    availability.push_back(time);
}

void Doctor::clearAvailability()
{
    availability.clear();
}