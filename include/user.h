#include "user.h"

User::User()
{
    id = 0;
    name = "";
    email = "";
    password = "";
    role = "";
}

User::User(int i, const std::string& n, const std::string& e, const std::string& p, const std::string& r)
    : id(i), name(n), email(e), password(p), role(r) {}

void User::set_id(int i) { id = i; }
void User::set_name(const std::string& n) { name = n; }
void User::set_password(const std::string& p) { password = p; }
void User::set_role(const std::string& r) { role = r; }
void User::set_email(const std::string& e) { email = e; }

int User::get_id() const { return id; }
std::string User::get_name() const { return name; }
std::string User::get_email() const { return email; }
std::string User::get_password() const { return password; }
std::string User::get_role() const { return role; }

bool User::Authenticate(const std::string& e, const std::string& p)
{
    return email == e && password == p;
}
