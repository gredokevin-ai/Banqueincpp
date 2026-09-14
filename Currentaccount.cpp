#include "Currentaccount.h"

CurrentAccount::CurrentAccount(std::string accountNumber, double balance, std::vector<Transaction> history, double overdraftLimit)
    : BankAccount(accountNumber, balance, history), m_overdraftLimit(overdraftLimit)
{
}
std::string CurrentAccount::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"accountNumber\": \"" << getAccountNumber() << "\", ";
    json << "\"balance\": " << getBalance() << ", ";
    json << "\"overdraftLimit\": " << m_overdraftLimit << ", ";
    json << "\"history\": [";
    const auto &history = getHistory();
    for (size_t i = 0; i < history.size(); ++i)
    {
        json << history[i].toJSON();
        if (i < history.size() - 1)
            json << ", ";
    }
    json << "]";
    json << "}";
    return json.str();
}
bool CurrentAccount::withdraw(double amount)
{
    if (amount > getBalance() + m_overdraftLimit)
    {
        std::cout << "Insufficient funds, including overdraft limit." << std::endl;
        return false;
    }
    setBalance(getBalance() - amount);
    Transaction transaction(getHistory().size() + 1, std::time(nullptr), amount, "Withdrawal", 0);
    std::vector<Transaction> history = getHistory();
    history.push_back(transaction);
    setHistory(history);
    return true;
}
double CurrentAccount::getOverdraftLimit() const
{
    return m_overdraftLimit;
}

void CurrentAccount::setOverdraftLimit(double overdraftLimit)
{
    m_overdraftLimit = overdraftLimit;
}

void CurrentAccount::endofyearprocessing()
{
    // Implementation for end-of-year processing for current account  : do nothing for current accounts
}

std::string CurrentAccount::getAccountType() const
{
    return "Courant";
}
