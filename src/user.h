#include <iostream>
using namespace std;
#ifndef USER_H
#define USER_H
class User
{
private:
    int id;
    string name;
    string email;
    string password;
    string role;

public:
    User();
    User(int i, string n, string e, string p, string r);
    void set_id(int i);
    void set_email(string e);
    void set_password(string p);
    void set_role(string r);
    void set_name(string n);
    int get_id();
    string get_email();
    string get_password();
    string get_role();
    string get_name();
    virtual ~User() {};
    virtual bool Authenticate(string Email, string password);
};
#endif