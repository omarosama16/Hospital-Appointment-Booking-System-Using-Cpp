#include <iostream>
#include "user.h"
#include "patient.h"
using namespace std;
patient::patient(int i, string n, string e, string p, string pa)
    : User(i, n, e, p, "Patient"), phone(pa) {}

string patient::get_phone()
{
    return phone;
}
void patient::set_phone(string p)
{
    phone = p;
}
 
