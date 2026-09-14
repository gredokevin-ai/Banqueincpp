#include "Savingsaccount.h"

SavingsAccount::SavingsAccount(std::string accountNumber, double balance, std::vector<Transaction> history, double interestRate)
    : BankAccount(accountNumber, balance, history), m_interestRate(interestRate)
{
}
std::string SavingsAccount::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"accountNumber\": \"" << getAccountNumber() << "\", ";
    json << "\"balance\": " << getBalance() << ", ";
    json << "\"interestRate\": " << m_interestRate << ", ";
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
bool SavingsAccount::withdraw(double amount)
{
    if (amount > getBalance())
    {
        std::cout << "Insufficient funds." << std::endl;
        return false;
    }
    setBalance(getBalance() - amount);
    Transaction transaction(getHistory().size() + 1, std::time(nullptr), amount, "Withdrawal", 0);
    std::vector<Transaction> history = getHistory();
    history.push_back(transaction);
    setHistory(history);
    return true;
}
double SavingsAccount::getInterestRate() const
{
    return m_interestRate;
}
void SavingsAccount::setInterestRate(double interestRate)
{
    m_interestRate = interestRate;
}
void SavingsAccount::applyInterest()
{
    double interest = getBalance() * m_interestRate;
    setBalance(getBalance() + interest);
}

void SavingsAccount::endofyearprocessing()
{
    applyInterest();
    std::cout << "Interest applied to savings account " << getAccountNumber() << ". New balance: " << getBalance() << std::endl;
}

std::string SavingsAccount::getAccountType() const
{
    return "Epargne";
}
