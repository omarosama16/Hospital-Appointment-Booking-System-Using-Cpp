#include "admin.h"

Admin::Admin(int id, string name, string email, string password)
    : User(id, name, email, password, "admin") {}