#include "Security.h"

std::string Security::hashPIN(const std::string &ID, const std::string &PIN)
{
    std::string combined = ID + "_" + PIN;
    int64_t hash = 14695981039346656037ULL; // Constante Offset Basis
    uint64_t prime = 1099511628211ULL;      // Constante FNV Prime
    for (char c : combined)
    {
        hash ^= static_cast<uint64_t>(c);
        hash *= prime;
    }
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << hash;
    return ss.str();
}
