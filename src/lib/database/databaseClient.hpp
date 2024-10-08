#ifndef DATABASE_HPP_
#define DATABASE_HPP_
#include <iostream>
#include <string>
#include <vector>
#include "soci/soci.h"

namespace database
{
    struct AccountRecord
    {
        unsigned int ID;
        std::string username;
        std::string password;
        AccountRecord()
        {
            ID = 0;
            username.clear();
            password.clear();
        }
        void reset()
        {
            ID = 0;
            username.clear();
            password.clear();
        }
        bool isEmpty() const
        {
            return ID == 0 && username.empty() && password.empty();
        }
    };

    struct ProfileRecord
    {
        unsigned int ID;
        unsigned int accountID;
        std::string name;
        std::string currency;
        double initialValue;
        ProfileRecord()
        {
            ID = 0;
            accountID = 0;
            name.clear();
            currency.clear();
            initialValue = 0.0;
        }
        void reset()
        {
            ID = 0;
            accountID = 0;
            name.clear();
            currency.clear();
            initialValue = 0.0;
        }
        bool isEmpty() const
        {
            return ID == 0 && accountID == 0 && name.empty() && currency.empty() && initialValue == 0.0;
        }
    };

    struct StockRecord
    {
        unsigned int ID;
        std::string name;
        std::string symbol;
        std::string currency;
        double currentPrice;
        std::string lastUpdate;
        StockRecord()
        {
            ID = 0;
            name.clear();
            symbol.clear();
            currency.clear();
            currentPrice = 0.0;
            lastUpdate.clear();
        }
        void reset()
        {
            ID = 0;
            name.clear();
            symbol.clear();
            currency.clear();
            currentPrice = 0.0;
            lastUpdate.clear();
        }
        bool isEmpty() const
        {
            return ID == 0 && name.empty() && symbol.empty() && currency.empty() && currentPrice == 0.0 && lastUpdate.empty();
        }
    };

    struct EquityRecord
    {
        unsigned int ID;
        unsigned int profileID;
        unsigned int stockID;
        double initialValue;
        double quantity;
        EquityRecord()
        {
            ID = 0;
            profileID = 0;
            stockID = 0;
            initialValue = 0.0;
            quantity = 0.0;
        }
        void reset()
        {
            ID = 0;
            profileID = 0;
            stockID = 0;
            initialValue = 0.0;
            quantity = 0.0;
        }
        bool isEmpty() const
        {
            return ID == 0 && profileID == 0 && stockID == 0 && initialValue == 0.0 && quantity == 0.0;
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
        virtual bool is_connected() = 0;
        virtual void reconnect() = 0;
        virtual bool insert(const AccountRecord &account) = 0;
        virtual bool update(const AccountRecord &account) = 0;
        virtual bool drop(const AccountRecord &account) = 0;
        virtual AccountRecord getAccount(const std::string &username, const std::string &password) = 0;
        virtual bool insert(const ProfileRecord &profile) = 0;
        virtual bool update(const ProfileRecord &profile) = 0;
        virtual bool drop(const ProfileRecord &profileID) = 0;
        virtual std::vector<ProfileRecord> getProfiles(const unsigned int &accountID) = 0;
        virtual bool insert(const StockRecord &stock) = 0;
        virtual bool update(const StockRecord &stock) = 0;
        virtual bool drop(const StockRecord &stockID) = 0;
        virtual StockRecord getStock(const std::string &symbol, const std::string &currency) = 0;
        virtual bool insert(const EquityRecord &equity) = 0;
        virtual bool update(const EquityRecord &equity) = 0;
        virtual bool drop(const EquityRecord &equityID) = 0;
        virtual std::vector<EquityRecord> getEquities(const unsigned int &profileID) = 0;
        virtual ~DatabaseClient() = default;
    };
};

namespace soci
{
    template <>
    struct type_conversion<database::AccountRecord>
    {
        typedef values base_type;

        static void from_base(values const &v, indicator /* ind */, database::AccountRecord &p)
        {
            p.ID = v.get<unsigned int>("ID", 0);
            p.username = v.get<std::string>("username", "");
            p.password = v.get<std::string>("password", "");
        }

        static void to_base(const database::AccountRecord &p, values &v, indicator &ind)
        {
            v.set("ID", p.ID);
            v.set("username", p.username);
            v.set("password", p.password);
        }
    };

    template <>
    struct type_conversion<database::ProfileRecord>
    {
        typedef values base_type;

        static void from_base(values const &v, indicator ind, database::ProfileRecord &p)
        {
            p.ID = v.get<unsigned int>("ID", 0);
            p.accountID = v.get<unsigned int>("account", 0);
            p.name = v.get<std::string>("name", "");
            p.currency = v.get<std::string>("currency", "");
            p.initialValue = v.get<double>("initialValue", 0);
        }

        static void to_base(const database::ProfileRecord &p, values &v, indicator &ind)
        {
            v.set("ID", p.ID);
            v.set("account", p.accountID);
            v.set("name", p.name);
            v.set("currency", p.currency);
            v.set("initialValue", p.initialValue);
        }
    };

    template <>
    struct type_conversion<database::StockRecord>
    {
        typedef values base_type;

        static void from_base(values const &v, indicator /* ind */, database::StockRecord &p)
        {
            p.ID = v.get<unsigned int>("ID", 0);
            p.name = v.get<std::string>("name", "");
            p.symbol = v.get<std::string>("symbol", "");
            p.currency = v.get<std::string>("currency", "");
            p.currentPrice = v.get<double>("currentPrice", 0);
            p.lastUpdate = v.get<std::string>("lastUpdate", "");
        }

        static void to_base(const database::StockRecord &p, values &v, indicator &ind)
        {
            v.set("ID", p.ID);
            v.set("name", p.name);
            v.set("symbol", p.symbol);
            v.set("currency", p.currency);
            v.set("currentPrice", p.currentPrice);
            v.set("lastUpdate", p.lastUpdate);
        }
    };

    template <>
    struct type_conversion<database::EquityRecord>
    {
        typedef values base_type;

        static void from_base(values const &v, indicator /* ind */, database::EquityRecord &p)
        {
            p.ID = v.get<unsigned int>("ID", 0);
            p.profileID = v.get<unsigned int>("profile", 0);
            p.stockID = v.get<unsigned int>("stock", 0);
            p.initialValue = v.get<double>("initialValue", 0);
            p.quantity = v.get<double>("quantity", 0);
        }

        static void to_base(const database::EquityRecord &p, values &v, indicator &ind)
        {
            v.set("ID", p.ID);
            v.set("profile", p.profileID);
            v.set("stock", p.stockID);
            v.set("initialValue", p.initialValue);
            v.set("quantity", p.quantity);
        }
    };
};

#endif