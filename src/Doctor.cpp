#include "Doctor.h"
#include <iostream>

using namespace std;

Doctor::Doctor(int id, string n, string e, string p, string s)
    : User(id, n, e, p, "doctor")
{
    specialization = s;
}

string Doctor::getSpecialization() const { return specialization; }

vector<string> Doctor::getAvailability() const { return availability; }

void Doctor::addAvailability(string time)
{
    availability.push_back(time);
}

void Doctor::clearAvailability()
{
    availability.clear();
}
