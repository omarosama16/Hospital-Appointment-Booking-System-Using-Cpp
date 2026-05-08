#pragma once

#include "user.h"
#include <string_view>

class Admin : public User
{
public:
    Admin(int id, std::string_view name, std::string_view email, std::string_view password);
};
