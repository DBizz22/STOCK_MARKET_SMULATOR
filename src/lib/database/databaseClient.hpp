#ifndef DATABASE_HPP_
#define DATABASE_HPP_
#include <iostream>
#include <string>
#include <vector>

struct accountDB
{
    unsigned int ID;
    std::string username;
    std::string password;
    void reset()
    {
        username.clear();
        password.clear();
    }
};

struct ProfileDB
{
    unsigned int ID;
    unsigned int accountID;
    std::string name;
    std::string currency;
    double initialValue;
    void reset()
    {
        accountID = 0;
        name.clear();
        currency.clear();
        initialValue = 0.0;
    }
};

struct StockDB
{
    unsigned int ID;
    std::string name;
    std::string symbol;
    std::string currency;
    double currentPrice;
    std::string lastUpdate;
    void reset()
    {
        name.clear();
        symbol.clear();
        currency.clear();
        currentPrice = 0.0;
        lastUpdate.clear();
    }
};

struct equityDB
{
    unsigned int ID;
    unsigned int profileID;
    unsigned int stockID;
    double initialValue;
    double quantity;
    void reset()
    {
        profileID = 0;
        stockID = 0;
        initialValue = 0.0;
        quantity = 0.0;
    }
};

class DatabaseClient
{
public:
    DatabaseClient() = default;
    DatabaseClient(DatabaseClient &copy) = default;
    DatabaseClient(DatabaseClient &&copy) noexcept = default;
    DatabaseClient &operator=(DatabaseClient &&copy) noexcept = default;
    DatabaseClient &operator=(const DatabaseClient &copy) = default;
    virtual bool insert(const accountDB &account) = 0;
    virtual bool update(const accountDB &account) = 0;
    virtual bool drop(const accountDB &account) = 0;
    virtual accountDB checkCredentials(const std::string &username, const std::string &password) = 0;
    virtual bool insert(const ProfileDB &profile) = 0;
    virtual bool update(const ProfileDB &profile) = 0;
    virtual bool drop(const ProfileDB &profileID) = 0;
    virtual std::vector<ProfileDB> getProfiles(const accountDB &account) = 0;
    virtual bool insert(const StockDB &stock) = 0;
    virtual bool update(const StockDB &stock) = 0;
    // virtual bool drop(const StockDB &stockID) = 0;
    virtual StockDB getStock(const std::string &symbol, const std::string &currency) = 0;
    virtual bool insert(const equityDB &equity) = 0;
    virtual bool update(const equityDB &equity) = 0;
    virtual bool drop(const equityDB &equityID) = 0;
    virtual std::vector<equityDB> getEquities(const ProfileDB &profileID) = 0;
    virtual ~DatabaseClient() = default;
};

#endif