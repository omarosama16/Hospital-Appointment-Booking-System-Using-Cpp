#ifndef USER_H
#define USER_H

#include <string>

class User
{
protected:
    int id;
    std::string name;
    std::string email;
    std::string password;
    std::string role;

public:
    User();
    User(int i, std::string n, std::string e, std::string p, std::string r);

    virtual ~User() {}

    void set_id(int i);
    void set_email(std::string e);
    void set_password(std::string p);
    void set_role(std::string r);
    void set_name(std::string n);

    int get_id() const;
    std::string get_email() const;
    std::string get_password() const;
    std::string get_role() const;
    std::string get_name() const;

    virtual bool Authenticate(std::string Email, std::string password);
};

#endif