#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    int id;
    std::string name;
    std::string email;
    std::string password;
    std::string role;

public:
    User();
    User(int i, std::string n, std::string e, std::string p, std::string r);

    void setId(int i);
    void setEmail(std::string e);
    void setPassword(std::string p);
    void setRole(std::string r);
    void setName(std::string n);

    int getId() const;
    std::string getEmail() const;
    std::string getPassword() const;
    std::string getRole() const;
    std::string getName() const;

    virtual ~User() {}

    virtual bool authenticate(std::string email, std::string password);
};

#endif
