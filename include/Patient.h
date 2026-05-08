#pragma once

#include "user.h"
#include <string>
#include <string_view>

class Patient : public User
{
public:
    Patient(int i, std::string_view n, std::string_view e,
            std::string_view p, std::string_view ph);

    void        set_phone(std::string_view p);
    std::string get_phone() const;

private:
    std::string phone;
};
