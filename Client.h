#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "BankAccount.h"
#include "Security.h"

#ifndef CLIENT_H
#define CLIENT_H

class Client
{
public:
    Client(int ID, std::string name, int PIN, std::vector<std::shared_ptr<BankAccount>> accounts);
    std::string toJSON() const;
    void addAccount(std::shared_ptr<BankAccount> account);
    void removeAccount(const std::string &accountNumber);
    void internalTransfer(const std::string &fromAccountNumber, const std::string &toAccountNumber, double amount);
    bool authentification(int PIN);
    int getID() const;
    std::string getName() const;
    std::string getHashedPIN() const;
    std::vector<std::shared_ptr<BankAccount>> getAccounts() const;
    void setID(int ID);
    void setName(const std::string &name);
    void setHashedPIN(const std::string &hashedPIN);
    void setAccounts(const std::vector<std::shared_ptr<BankAccount>> &accounts);
    void displayAccounts() const;
    BankAccount *chercherCompte(const std::string &accountNumber);

private:
    int m_ID;
    std::string m_name;
    std::string m_hashedPIN;
    // int m_PIN;
    std::vector<std::shared_ptr<BankAccount>> m_accounts;
};
#endif // CLIENT_H
