#include "user.h"

User::User()
{
    id = 0;
    name = "";
    email = "";
    password = "";
    role = "";
}

User::User(int i, std::string_view n, std::string_view e, std::string_view p, std::string_view r)
    : id(i), name(n), email(e), password(p), role(r) {}

void User::set_id(int i)               { id = i; }
void User::set_name(std::string_view n)     { name = n; }
void User::set_password(std::string_view p) { password = p; }
void User::set_role(std::string_view r)     { role = r; }
void User::set_email(std::string_view e)    { email = e; }

int         User::get_id()       const { return id; }
std::string User::get_name()     const { return name; }
std::string User::get_email()    const { return email; }
std::string User::get_password() const { return password; }
std::string User::get_role()     const { return role; }

bool User::Authenticate(std::string_view e, std::string_view p)
{
    return email == e && password == p;
}
