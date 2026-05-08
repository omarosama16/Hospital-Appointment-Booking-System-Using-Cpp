#pragma once

#include "user.h"
#include <string>
#include <string_view>
#include <vector>

class Doctor : public User
{
public:
    Doctor(int i, std::string_view n, std::string_view e,
           std::string_view p, std::string_view s);

    std::string              getSpecialization() const;
    std::vector<std::string> getAvailability()   const;

    void addAvailability(std::string_view time);
    void clearAvailability();

private:
    std::string              specialization;
    std::vector<std::string> availability;
};
