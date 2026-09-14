#include "Bankaccount.h"

BankAccount::BankAccount(std::string accountNumber, double balance, std::vector<Transaction> history)
    : m_accountNumber(accountNumber), m_balance(balance), m_history(history)
{
}
std::string BankAccount::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"accountNumber\": \"" << m_accountNumber << "\", ";
    json << "\"balance\": " << m_balance << ", ";
    json << "\"history\": [";
    for (size_t i = 0; i < m_history.size(); ++i)
    {
        json << m_history[i].toJSON();
        if (i < m_history.size() - 1)
            json << ", ";
    }
    json << "]";
    json << "}";
    return json.str();
}
void BankAccount::deposit(double amount)
{
    m_balance += amount;
    Transaction transaction(m_history.size() + 1, std::time(nullptr), amount, "Deposit", 0);
    m_history.push_back(transaction);
}
bool BankAccount::withdraw(double amount)
{
    if (amount > m_balance)
    {
        std::cout << "Insufficient funds." << std::endl;
        return false;
    }
    m_balance -= amount;
    Transaction transaction(m_history.size() + 1, std::time(nullptr), amount, "Withdrawal", 0);
    m_history.push_back(transaction);
    return true;
}

void BankAccount::displayhistory() const
{
    std::cout << "Transaction History for Account " << m_accountNumber << ":" << std::endl;
    for (const auto &transaction : m_history)
    {
        std::cout << "ID: " << transaction.getID()
                  << ", Timestamp: " << transaction.getTimestamp()
                  << ", Amount: " << transaction.getAmount()
                  << ", Type: " << transaction.getType()
                  << ", Libel: " << transaction.getLibel() << std::endl;
    }
}

std::string BankAccount::getAccountNumber() const
{
    return m_accountNumber;
}

void BankAccount::setAccountNumber(const std::string &accountNumber)
{
    m_accountNumber = accountNumber;
}

double BankAccount::getBalance() const
{
    return m_balance;
}

void BankAccount::setBalance(double balance)
{
    m_balance = balance;
}

std::vector<Transaction> BankAccount::getHistory() const
{
    return m_history;
}

void BankAccount::setHistory(const std::vector<Transaction> &history)
{
    m_history = history;
}

void BankAccount::endofyearprocessing()
{
    // Default implementation does nothing. Derived classes can override this method.
}
std::string BankAccount::getAccountType() const
{
    return "Generic Bank Account";
}
