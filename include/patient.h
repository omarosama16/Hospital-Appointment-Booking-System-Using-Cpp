#ifndef PATIENT_H
#define PATIENT_H
#include "user.h"
#include <string>
using namespace std;

class patient : public User
{
private:
    string phone;
public:
    patient(int id, string name, string email, string password, string phone);
    void set_phone(string p);
    string get_phone() const;
    ~patient() {}
};
#endif
