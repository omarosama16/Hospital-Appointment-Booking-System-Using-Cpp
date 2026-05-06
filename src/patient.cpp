#include "Patient.h"

Patient::Patient(int i, std::string n, std::string e, std::string p, std::string ph)
    : User(i, n, e, p, "patient"), phone(ph) {}

void Patient::set_phone(std::string p)
{
    phone = p;
}

std::string Patient::get_phone() const
{
    return phone;
}