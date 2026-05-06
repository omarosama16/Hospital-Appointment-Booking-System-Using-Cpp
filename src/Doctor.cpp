#include "Doctor.h"

Doctor::Doctor(int i, std::string n, std::string e, std::string p, std::string s)
    : User(i, n, e, p, "doctor"), specialization(s) {}

std::string Doctor::getSpecialization() const
{
    return specialization;
}

std::vector<std::string> Doctor::getAvailability() const
{
    return availability;
}

void Doctor::addAvailability(std::string time)
{
    availability.push_back(time);
}

void Doctor::clearAvailability()
{
    availability.clear();
}