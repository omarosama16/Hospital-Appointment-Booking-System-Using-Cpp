#include "user.h"

User::User()
{
    id = 0;
    name = "";
    email = "";
    password = "";
    role = "";
}

User::User(int i, std::string n, std::string e, std::string p, std::string r)
    : id(i), name(n), email(e), password(p), role(r) {}

void User::set_id(int i) { id = i; }
void User::set_name(std::string n) { name = n; }
void User::set_password(std::string p) { password = p; }
void User::set_role(std::string r) { role = r; }
void User::set_email(std::string e) { email = e; }

int User::get_id() const { return id; }
std::string User::get_name() const { return name; }
std::string User::get_email() const { return email; }
std::string User::get_password() const { return password; }
std::string User::get_role() const { return role; }

bool User::Authenticate(std::string e, std::string p)
{
    return email == e && password == p;
}