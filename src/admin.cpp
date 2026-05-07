#include "admin.h"

Admin::Admin(int id, std::string name, std::string email, std::string password)
    : User(id, name, email, password, "admin")
{

    set_role("admin");
}