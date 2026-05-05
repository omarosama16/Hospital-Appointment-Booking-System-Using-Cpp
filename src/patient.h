#include <iostream>
#include "Appointment.h"
#include <vector>
#include "user.h"
using namespace std;
#ifndef PATIENT_H
#define PATIENT_H
class patient : public User
{
private:
    string phone;

public:
    patient(int id, string name, string email, string password, string phone);
    void set_phone(string pa);
    string get_phone();
    ~patient() {};
     
};
#endif
