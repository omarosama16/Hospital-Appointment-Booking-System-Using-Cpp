#ifndef ADMIN_H
#define ADMIN_H
#include "user.h"
#include <string>
using namespace std;

class Admin : public User
{
public:
    Admin(int id, string name, string email, string password);
};
#endif
