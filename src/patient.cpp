#include "Patient.h"

Patient::Patient(int i, string n, string e, string p, string ph)
    : User(i, n, e, p, "patient"), phone(ph) {}

void Patient::set_phone(string p) { phone = p; }

string Patient::get_phone() const { return phone; }