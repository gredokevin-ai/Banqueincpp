#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Client.h"
#include "BankAccount.h"
#include "Currentaccount.h"
#include "Savingsaccount.h"
#include "Transaction.h"

#ifndef BANK_H
#define BANK_H

class Bank
{
public:
    Bank(std::string name);
    Bank() = default;
    std::string toJSON() const;
    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
    void addclient(const Client &client);
    Client *getClientByID(int ID);
    std::string createCurrentAccount(int clientID, double initialBalance, double overdraftLimit); // Crée un compte courant pour le client spécifié et retourne le numéro de compte
    std::string createSavingsAccount(int clientID, double initialBalance, double interestRate);   // Crée un compte d'épargne pour le client spécifié et retourne le numéro de compte
    std::shared_ptr<BankAccount> getAccountByNumber(const std::string &accountNumber);
    bool maketransfer(const std::string &fromAccountNumber, const std::string &toAccountNumber, double amount);
    void simulateendyear();
    void displayBankInfo() const;

private:
    std::string m_name;
    int m_clientNumber;
    std::unordered_map<int, Client> ClientRegister; // Map des clients avec leur identifiant comme clé
};
#endif
