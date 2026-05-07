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
    User(int i, const std::string& n, const std::string& e, const std::string& p, const std::string& r);
    virtual ~User() {}

    void set_id(int i);
    void set_email(const std::string& e);
    void set_password(const std::string& p);
    void set_role(const std::string& r);
    void set_name(const std::string& n);

    int get_id() const;
    std::string get_email() const;
    std::string get_password() const;
    std::string get_role() const;
    std::string get_name() const;

    virtual bool Authenticate(const std::string& email, const std::string& password);
};

#endif
