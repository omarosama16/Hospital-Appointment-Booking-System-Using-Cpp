#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

class Admin : public User
{
public:
    Admin(int id, std::string name, std::string email, std::string password);
};

#endif