#ifndef PATIENT_H
#define PATIENT_H

#include "user.h"

class Patient : public User
{
private:
    string phone;

public:
    Patient(int id, string name, string email, string password, string phone);

    void set_phone(string p);
    string get_phone() const;

    ~Patient() {}
};

#endif