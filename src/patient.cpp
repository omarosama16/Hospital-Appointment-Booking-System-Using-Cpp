#include "Patient.h"

Patient::Patient(int i, std::string_view n, std::string_view e,
                 std::string_view p, std::string_view ph)
    : User(i, n, e, p, "patient"), phone(ph) {}

void Patient::set_phone(std::string_view p)
{
    phone = p;
}

std::string Patient::get_phone() const
{
    return phone;
}
