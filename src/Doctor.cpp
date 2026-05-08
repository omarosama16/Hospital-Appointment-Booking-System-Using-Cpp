#include "Doctor.h"

Doctor::Doctor(int i, std::string_view n, std::string_view e,
               std::string_view p, std::string_view s)
    : User(i, n, e, p, "doctor"), specialization(s) {}

std::string Doctor::getSpecialization() const
{
    return specialization;
}

std::vector<std::string> Doctor::getAvailability() const
{
    return availability;
}

void Doctor::addAvailability(std::string_view time)
{
    availability.emplace_back(time);
}

void Doctor::clearAvailability()
{
    availability.clear();
}
