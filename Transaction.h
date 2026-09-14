#include <string>
#include <sstream>
#include <ctime>
#ifndef TRANSACTION_H
#define TRANSACTION_H

// Représente une opération effectuée sur un compte.
class Transaction
{
public:
    Transaction(int ID = 0, std::time_t timestamp = 0.0,
                double amount = 0.0, std::string type = "", int libel = 0);
    std::string toJSON() const;

    int getID() const;
    void setID(int ID);

    std::time_t getTimestamp() const;
    void setTimestamp(std::time_t timestamp);

    double getAmount() const;
    void setAmount(double amount);

    std::string getType() const;
    void setType(const std::string &type);

    int getLibel() const;
    void setLibel(int libel);

private:
    int m_ID;
    double m_timestamp;
    double m_amount;
    std::string m_type;
    int m_libel;
};

#endif
