#include "Client.h"

Client::Client(int ID, std::string name, int PIN, std::vector<std::shared_ptr<BankAccount>> accounts)
    : m_ID(ID), m_name(name), m_accounts(accounts)
{
    this->m_hashedPIN = Security::hashPIN(std::to_string(ID), std::to_string(PIN));
}
std::string Client::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"ID\": " << m_ID << ", ";
    json << "\"name\": \"" << m_name << "\", ";
    json << "\"hashedPIN\": \"" << m_hashedPIN << "\", ";
    json << "\"accounts\": [";
    for (size_t i = 0; i < m_accounts.size(); ++i)
    {
        json << m_accounts[i]->toJSON();
        if (i < m_accounts.size() - 1)
            json << ", ";
    }
    json << "]";
    json << "}";
    return json.str();
}

void Client::addAccount(std::shared_ptr<BankAccount> account)
{
    m_accounts.push_back(account);
}

void Client::removeAccount(const std::string &accountNumber)
{
    for (auto it = m_accounts.begin(); it != m_accounts.end(); ++it)
    {
        if ((*it)->getAccountNumber() == accountNumber)
        {
            m_accounts.erase(it);
            break;
        }
    }
}

void Client::internalTransfer(const std::string &fromAccountNumber, const std::string &toAccountNumber, double amount)
{
    std::shared_ptr<BankAccount> fromAccount = nullptr;
    std::shared_ptr<BankAccount> toAccount = nullptr;

    for (const auto &account : m_accounts)
    {
        if (account->getAccountNumber() == fromAccountNumber)
        {
            fromAccount = account;
        }
        if (account->getAccountNumber() == toAccountNumber)
        {
            toAccount = account;
        }
    }

    if (fromAccount && toAccount)
    {
        fromAccount->withdraw(amount);
        toAccount->deposit(amount);
    }
}

bool Client::authentification(int PIN)
{
    std::string hashedInputPIN = Security::hashPIN(std::to_string(m_ID), std::to_string(PIN));
    return m_hashedPIN == hashedInputPIN;
}

int Client::getID() const
{
    return m_ID;
}

std::string Client::getName() const
{
    return m_name;
}

std::string Client::getHashedPIN() const
{
    return m_hashedPIN;
}

std::vector<std::shared_ptr<BankAccount>> Client::getAccounts() const
{
    return m_accounts;
}

void Client::setID(int ID)
{
    m_ID = ID;
}

void Client::setName(const std::string &name)
{
    m_name = name;
}

void Client::setHashedPIN(const std::string &hashedPIN)
{
    m_hashedPIN = hashedPIN;
}

void Client::setAccounts(const std::vector<std::shared_ptr<BankAccount>> &accounts)
{
    m_accounts = accounts;
}

void Client::displayAccounts() const
{
    std::cout << "Comptes de " << m_name << " (ID: " << m_ID << "):\n";
    for (const auto &account : m_accounts)
    {
        std::cout << "  - Compte " << account->getAccountType() << " n°" << account->getAccountNumber() << ", Solde: " << account->getBalance() << "\n";
    }
}

BankAccount *Client::chercherCompte(const std::string &accountNumber)
{
    for (const auto &account : m_accounts)
    {
        if (account->getAccountNumber() == accountNumber)
        {
            return account.get();
        }
    }
    return nullptr;
}
