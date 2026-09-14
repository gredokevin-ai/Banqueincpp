#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Transaction.h"
#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

class BankAccount
{
public:
    BankAccount(std::string accountNumber, double balance, std::vector<Transaction> history);
    virtual std::string toJSON() const;
    void deposit(double amount);
    virtual bool withdraw(double amount);
    void displayhistory() const;
    std::string getAccountNumber() const;
    void setAccountNumber(const std::string &accountNumber);
    double getBalance() const;
    void setBalance(double balance);
    std::vector<Transaction> getHistory() const;
    void setHistory(const std::vector<Transaction> &history);
    virtual void endofyearprocessing();
    virtual std::string getAccountType() const;

private:
    std::string m_accountNumber;
    double m_balance;
    std::vector<Transaction> m_history;
};
#endif
