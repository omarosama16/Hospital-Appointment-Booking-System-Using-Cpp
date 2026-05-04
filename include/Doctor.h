#ifndef DOCTOR_H
#define DOCTOR_H

#include "user.h"
#include <string>
#include <vector>
using namespace std;

class Doctor : public User
{
private:
    string specialization;
    vector<string> availability;

public:
    Doctor(int id, string name, string email, string password, string specialization);

    string getSpecialization() const;
    vector<string> getAvailability() const;

    void addAvailability(string time);
    void clearAvailability();

   
};

#endif