#pragma once

#include <string>
#include <string_view>

class User
{
public:
    User();
    User(int i, std::string_view n, std::string_view e,
         std::string_view p, std::string_view r);

    virtual ~User() = default;

    void set_id(int i);
    void set_name(std::string_view n);
    void set_email(std::string_view e);
    void set_password(std::string_view p);
    void set_role(std::string_view r);

    int         get_id()       const;
    std::string get_name()     const;
    std::string get_email()    const;
    std::string get_password() const;
    std::string get_role()     const;

    // Fix: declared const — only reads email and password, never modifies them
    bool Authenticate(std::string_view e, std::string_view p) const;

protected:
    int         id;
    std::string name;
    std::string email;
    std::string password;
    std::string role;
};
