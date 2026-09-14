#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Bankaccount.h"

#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

class SavingsAccount : public BankAccount
{
public:
    SavingsAccount(std::string accountNumber, double balance, std::vector<Transaction> history, double interestRate);
    std::string toJSON() const override;
    bool withdraw(double amount) override;
    double getInterestRate() const;
    void setInterestRate(double interestRate);
    void applyInterest();                // Méthode pour appliquer les intérêts au solde du compte
    void endofyearprocessing() override; // Méthode pour le traitement de fin d'année spécifique aux comptes d'épargne
    std::string getAccountType() const override;

private:
    double m_interestRate;
};
#endif
