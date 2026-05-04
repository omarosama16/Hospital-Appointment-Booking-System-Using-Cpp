#include "admin.h"
using namespace std;

Admin::Admin(int id, string name, string email, string password)
    : User(id, name, email, password, "Admin") {}
