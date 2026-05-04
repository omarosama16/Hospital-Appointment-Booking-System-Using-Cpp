#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"
#include <string>

class Patient : public User
{
private:
    std::string phone;

public:
    Patient(int id, std::string name, std::string email, std::string password, std::string phone);
    void setPhone(std::string p);
    std::string getPhone() const;

    ~Patient() {}
};

#endif
