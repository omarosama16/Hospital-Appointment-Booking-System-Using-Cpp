#ifndef PATIENT_H
#define PATIENT_H

#include "user.h"

class Patient : public User
{
private:
    std::string phone;

public:
    Patient(int id, std::string name, std::string email, std::string password, std::string phone);

    void set_phone(std::string p);
    std::string get_phone() const;
};

#endif