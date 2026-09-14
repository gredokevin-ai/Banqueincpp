#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Bankaccount.h"

#ifndef CURRENTACCOUNT_H
#define CURRENTACCOUNT_H

class CurrentAccount : public BankAccount
{
public:
    CurrentAccount(std::string accountNumber, double balance, std::vector<Transaction> history, double overdraftLimit);
    std::string toJSON() const override;
    bool withdraw(double amount) override;
    double getOverdraftLimit() const;
    void setOverdraftLimit(double overdraftLimit);
    void endofyearprocessing() override;
    std::string getAccountType() const override;

private:
    double m_overdraftLimit;
};
#endif
