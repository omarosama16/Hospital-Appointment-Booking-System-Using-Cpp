#ifndef DOCTOR_H
#define DOCTOR_H

#include "user.h"
#include <vector>

class Doctor : public User
{
private:
    std::string specialization;
    std::vector<std::string> availability;

public:
    Doctor(int id, std::string name, std::string email, std::string password, std::string specialization);

    std::string getSpecialization() const;

    std::vector<std::string> getAvailability() const;

    void addAvailability(std::string time);
    void clearAvailability();
};

#endif