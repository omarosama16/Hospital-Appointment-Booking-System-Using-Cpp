#include "admin.h"

Admin::Admin(int id, std::string_view name, std::string_view email, std::string_view password)
    : User(id, name, email, password, "admin")
{
}
