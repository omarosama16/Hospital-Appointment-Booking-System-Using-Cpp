#include <iostream>
#include "user.h"
using namespace std;

User::User() // default constructor must be  implemented
{
    id = 0;
    name = "";
    email = "";
    password = "";
    role = "";
}
User::User(int i, string n, string e, string p, string r) : id(i), name(n), email(e), password(p), role(r) {}
void User::set_id(int i)
{
    id = i;
}
void User::set_name(string n)
{
    name = n;
}
void User::set_password(string p)
{
    password = p;
}
void User::set_role(string r)
{
    role = r;
}
void User::set_email(string e)
{
    email = e;
}

int User::get_id()
{
    return id;
}
string User::get_name()
{
    return name;
}
string User::get_password()
{
    return password;
}
string User::get_role()
{
    return role;
}
string User::get_email()
{
    return email;
}

bool User::Authenticate(string email, string password) // we will ensure that the user credentials is true
{
    return (this->email == email && this->password == password);
}
