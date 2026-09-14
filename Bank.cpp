#include "Bank.h"

#include <cctype>
#include <map>
#include <variant>

namespace
{
    struct JsonValue
    {
        using Object = std::map<std::string, JsonValue>;
        using Array = std::vector<JsonValue>;
        using Value = std::variant<std::nullptr_t, bool, double, std::string, Object, Array>;

        Value value;
    };

    class JsonParser
    {
    public:
        explicit JsonParser(const std::string &json) : m_json(json), m_position(0) {}

        JsonValue parse()
        {
            JsonValue result = parseValue();
            skipWhitespace();
            if (m_position != m_json.size())
                throw std::runtime_error("Unexpected characters after JSON document");
            return result;
        }

    private:
        const std::string &m_json;
        size_t m_position;

        void skipWhitespace()
        {
            while (m_position < m_json.size() && std::isspace(static_cast<unsigned char>(m_json[m_position])))
                ++m_position;
        }

        char next()
        {
            if (m_position >= m_json.size())
                throw std::runtime_error("Unexpected end of JSON document");
            return m_json[m_position++];
        }

        void expect(char expected)
        {
            if (next() != expected)
                throw std::runtime_error("Invalid JSON syntax");
        }

        std::string parseString()
        {
            expect('"');
            std::string result;
            while (m_position < m_json.size())
            {
                char character = next();
                if (character == '"')
                    return result;
                if (character == '\\')
                {
                    char escaped = next();
                    switch (escaped)
                    {
                    case '"':
                    case '\\':
                    case '/':
                        result += escaped;
                        break;
                    case 'b':
                        result += '\b';
                        break;
                    case 'f':
                        result += '\f';
                        break;
                    case 'n':
                        result += '\n';
                        break;
                    case 'r':
                        result += '\r';
                        break;
                    case 't':
                        result += '\t';
                        break;
                    default:
                        throw std::runtime_error("Unsupported JSON escape sequence");
                    }
                }
                else
                {
                    result += character;
                }
            }
            throw std::runtime_error("Unterminated JSON string");
        }

        double parseNumber()
        {
            size_t length = 0;
            double result = std::stod(m_json.substr(m_position), &length);
            m_position += length;
            return result;
        }

        JsonValue parseValue()
        {
            skipWhitespace();
            if (m_position >= m_json.size())
                throw std::runtime_error("Missing JSON value");

            switch (m_json[m_position])
            {
            case '{':
                return parseObject();
            case '[':
                return parseArray();
            case '"':
                return {parseString()};
            case 't':
                if (m_json.compare(m_position, 4, "true") == 0)
                {
                    m_position += 4;
                    return {true};
                }
                break;
            case 'f':
                if (m_json.compare(m_position, 5, "false") == 0)
                {
                    m_position += 5;
                    return {false};
                }
                break;
            case 'n':
                if (m_json.compare(m_position, 4, "null") == 0)
                {
                    m_position += 4;
                    return {nullptr};
                }
                break;
            default:
                if (m_json[m_position] == '-' || std::isdigit(static_cast<unsigned char>(m_json[m_position])))
                    return {parseNumber()};
            }
            throw std::runtime_error("Invalid JSON value");
        }

        JsonValue parseObject()
        {
            expect('{');
            JsonValue::Object object;
            skipWhitespace();
            if (m_position < m_json.size() && m_json[m_position] == '}')
            {
                ++m_position;
                return {object};
            }
            while (true)
            {
                skipWhitespace();
                std::string key = parseString();
                skipWhitespace();
                expect(':');
                object.emplace(std::move(key), parseValue());
                skipWhitespace();
                char separator = next();
                if (separator == '}')
                    return {object};
                if (separator != ',')
                    throw std::runtime_error("Invalid JSON object");
            }
        }

        JsonValue parseArray()
        {
            expect('[');
            JsonValue::Array array;
            skipWhitespace();
            if (m_position < m_json.size() && m_json[m_position] == ']')
            {
                ++m_position;
                return {array};
            }
            while (true)
            {
                array.push_back(parseValue());
                skipWhitespace();
                char separator = next();
                if (separator == ']')
                    return {array};
                if (separator != ',')
                    throw std::runtime_error("Invalid JSON array");
            }
        }
    };

    const JsonValue::Object &jsonObject(const JsonValue &value)
    {
        return std::get<JsonValue::Object>(value.value);
    }

    const JsonValue::Array &jsonArray(const JsonValue &value)
    {
        return std::get<JsonValue::Array>(value.value);
    }

    double jsonNumber(const JsonValue::Object &object, const std::string &key)
    {
        return std::get<double>(object.at(key).value);
    }

    const std::string &jsonString(const JsonValue::Object &object, const std::string &key)
    {
        return std::get<std::string>(object.at(key).value);
    }
}

Bank::Bank(std::string name) : m_name(name), m_clientNumber(0) {}
// Bank::Bank() : m_name("Default Bank"), m_clientNumber(0) {}
std::string Bank::toJSON() const
{
    std::stringstream json;
    json << "{";
    json << "\"name\": \"" << m_name << "\", ";
    json << "\"clients\": [";
    for (auto it = ClientRegister.begin(); it != ClientRegister.end(); ++it)
    {
        json << it->second.toJSON();
        if (std::next(it) != ClientRegister.end())
            json << ", ";
    }
    json << "]";
    json << "}";
    return json.str();
}
bool Bank::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }
    else
    {
        file << toJSON();
        file.close();
        return true;
    }
}
bool Bank::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return false;
    }
    else
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        try
        {
            JsonValue root = JsonParser(buffer.str()).parse();
            const auto &rootObject = jsonObject(root);
            const auto &clients = jsonArray(rootObject.at("clients"));
            std::unordered_map<int, Client> loadedClients;

            for (const auto &clientValue : clients)
            {
                const auto &clientObject = jsonObject(clientValue);
                std::vector<std::shared_ptr<BankAccount>> accounts;

                for (const auto &accountValue : jsonArray(clientObject.at("accounts")))
                {
                    const auto &accountObject = jsonObject(accountValue);
                    std::vector<Transaction> history;
                    for (const auto &transactionValue : jsonArray(accountObject.at("history")))
                    {
                        const auto &transactionObject = jsonObject(transactionValue);
                        history.emplace_back(
                            static_cast<int>(jsonNumber(transactionObject, "ID")),
                            static_cast<std::time_t>(jsonNumber(transactionObject, "timestamp")),
                            jsonNumber(transactionObject, "amount"),
                            jsonString(transactionObject, "type"),
                            static_cast<int>(jsonNumber(transactionObject, "libel")));
                    }

                    const std::string accountNumber = jsonString(accountObject, "accountNumber");
                    const double balance = jsonNumber(accountObject, "balance");
                    if (accountObject.find("overdraftLimit") != accountObject.end())
                    {
                        accounts.push_back(std::make_shared<CurrentAccount>(
                            accountNumber, balance, history, jsonNumber(accountObject, "overdraftLimit")));
                    }
                    else if (accountObject.find("interestRate") != accountObject.end())
                    {
                        accounts.push_back(std::make_shared<SavingsAccount>(
                            accountNumber, balance, history, jsonNumber(accountObject, "interestRate")));
                    }
                    else
                    {
                        throw std::runtime_error("Unknown bank account type");
                    }
                }

                Client client(
                    static_cast<int>(jsonNumber(clientObject, "ID")),
                    jsonString(clientObject, "name"),
                    static_cast<int>(jsonNumber(clientObject, "PIN")),
                    std::move(accounts));
                loadedClients.emplace(client.getID(), std::move(client));
            }

            m_name = jsonString(rootObject, "name");
            ClientRegister = std::move(loadedClients);
            m_clientNumber = static_cast<int>(ClientRegister.size());
            return true;
        }
        catch (const std::exception &error)
        {
            std::cerr << "Error parsing JSON file " << filename << ": " << error.what() << std::endl;
            return false;
        }
    }
}
void Bank::addclient(const Client &client)
{
    m_clientNumber++;
    if (ClientRegister.find(client.getID()) != ClientRegister.end())
    {
        throw std::runtime_error("Client with ID " + std::to_string(client.getID()) + " already exists.");
    }
    else
    {
        ClientRegister.try_emplace(client.getID(), client);
    }
}
Client *Bank::getClientByID(int ID)
{
    auto it = ClientRegister.find(ID);
    if (it != ClientRegister.end())
    {
        return &it->second;
    }
    return nullptr;
}
std::string Bank::createCurrentAccount(int clientID, double initialBalance, double overdraftLimit)
{
    Client *client = getClientByID(clientID);
    if (client)
    {
        std::shared_ptr<CurrentAccount> newCurrentAccount = std::make_shared<CurrentAccount>(std::to_string(clientID) +
                                                                                                 "_current_" + std::to_string(client->getAccounts().size() + 1),
                                                                                             initialBalance, std::vector<Transaction>(),
                                                                                             overdraftLimit);
        client->addAccount(newCurrentAccount);
        return newCurrentAccount->getAccountNumber();
    }
    return "-1";
}

std::string Bank::createSavingsAccount(int clientID, double initialBalance, double interestRate)
{
    Client *client = getClientByID(clientID);
    if (client)
    {
        std::shared_ptr<SavingsAccount> newSavingsAccount = std::make_shared<SavingsAccount>(std::to_string(clientID) +
                                                                                                 "_savings_" + std::to_string(client->getAccounts().size() + 1),
                                                                                             initialBalance, std::vector<Transaction>(),
                                                                                             interestRate);
        client->addAccount(newSavingsAccount);
        return newSavingsAccount->getAccountNumber();
    }
    return "-1";
}

std::shared_ptr<BankAccount> Bank::getAccountByNumber(const std::string &accountNumber)
{
    for (const auto &[key, client] : ClientRegister)
    {
        for (const auto &account : client.getAccounts())
        {
            if (account->getAccountNumber() == accountNumber)
            {
                return account;
            }
        }
    }
    return nullptr;
}

bool Bank::maketransfer(const std::string &fromAccountNumber, const std::string &toAccountNumber, double amount)
{
    std::shared_ptr<BankAccount> fromAccount = getAccountByNumber(fromAccountNumber);
    std::shared_ptr<BankAccount> toAccount = getAccountByNumber(toAccountNumber);

    if (fromAccount && toAccount)
    {
        bool success = fromAccount->withdraw(amount);
        if (success)
        {
            toAccount->deposit(amount);
            std::cout << "Transfer of " << amount << " from account " << fromAccountNumber
                      << " to account " << toAccountNumber << " completed successfully." << std::endl;
        }
        else
        {
            std::cout << "Transfer of " << amount << " from account " << fromAccountNumber
                      << " to account " << toAccountNumber << " failed. Insufficient funds." << std::endl;
        }
        return success;
    }
    return false;
}

void Bank::simulateendyear()
{
    for (const auto &[key, client] : ClientRegister)
    {
        for (const auto &account : client.getAccounts())
        {
            account->endofyearprocessing();
        }
    }
}

void Bank::displayBankInfo() const
{
    std::cout << "Bank Name: " << m_name << std::endl;
    std::cout << "Number of Clients: " << ClientRegister.size() << std::endl;
    for (const auto &[key, client] : ClientRegister)
    {
        std::cout << "Client ID: " << client.getID() << ", Name: " << client.getName() << std::endl;
        for (const auto &account : client.getAccounts())
        {
            std::cout << "  Account Number: " << account->getAccountNumber()
                      << ", Balance: " << account->getBalance() << std::endl;
        }
    }
}
