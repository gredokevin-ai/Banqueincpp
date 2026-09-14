#include "Transaction.h"

Transaction::Transaction(int ID, std::time_t timestamp, double amount,
                         std::string type, int libel)
    : m_ID(ID),
      m_timestamp(timestamp),
      m_amount(amount),
      m_type(type),
      m_libel(libel)
{
}
std::string Transaction::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"ID\": " << m_ID << ", ";
    json << "\"timestamp\": " << m_timestamp << ", ";
    json << "\"amount\": " << m_amount << ", ";
    json << "\"type\": \"" << m_type << "\", ";
    json << "\"libel\": " << m_libel;
    json << "}";
    return json.str();
}

int Transaction::getID() const
{
    return m_ID;
}

void Transaction::setID(int ID)
{
    m_ID = ID;
}

std::time_t Transaction::getTimestamp() const
{
    return m_timestamp;
}

void Transaction::setTimestamp(std::time_t timestamp)
{
    m_timestamp = timestamp;
}

double Transaction::getAmount() const
{
    return m_amount;
}

void Transaction::setAmount(double amount)
{
    m_amount = amount;
}

std::string Transaction::getType() const
{
    return m_type;
}

void Transaction::setType(const std::string &type)
{
    m_type = type;
}

int Transaction::getLibel() const
{
    return m_libel;
}

void Transaction::setLibel(int libel)
{
    m_libel = libel;
}
