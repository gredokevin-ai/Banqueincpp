#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

#ifndef SECURITY_H
#define SECURITY_H
class Security
{
public:
    Security();
    static std::string hashPIN(const std::string &ID, const std::string &PIN);

private:
};

#endif // SECURITY_H
