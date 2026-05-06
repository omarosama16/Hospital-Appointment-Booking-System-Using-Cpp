#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
protected:
    int id;
    string name;
    string email;
    string password;
    string role;

public:
    User();
    User(int i, string n, string e, string p, string r);

    virtual ~User() {}

    void set_id(int i);
    void set_email(string e);
    void set_password(string p);
    void set_role(string r);
    void set_name(string n);

    int get_id() const;
    string get_email() const;
    string get_password() const;
    string get_role() const;
    string get_name() const;

    virtual bool Authenticate(string Email, string password);
};

#endif