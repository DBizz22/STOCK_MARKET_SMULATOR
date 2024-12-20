#ifndef MYSQLCLIENT_HPP_
#define MYSQLCLIENT_HPP_

#include "databaseClient.hpp"
#include "soci/soci.h"
#include "soci/mysql/soci-mysql.h"

namespace database
{
    namespace mysql
    {
        struct connectionParams
        {
            std::string database;
            std::string user;
            std::string password;
        };

        class MySQLClient : public DatabaseClient
        {
        private:
            connectionParams credentials_;
            soci::session sql;
            bool databaseReady;
            void setupConnection();
            bool inputQuery(const std::string &query);
            template <typename T>
            T singleOutputQuery(const std::string &query);
            template <typename T>
            std::vector<T> multipleOutputQuery(const std::string &query);

        public:
            explicit MySQLClient(const connectionParams &credentials);
            MySQLClient(MySQLClient &copy) = default;
            MySQLClient(MySQLClient &&copy) noexcept = default;
            MySQLClient &operator=(MySQLClient &&copy) noexcept = default;
            MySQLClient &operator=(const MySQLClient &copy) = default;
            void reset(const connectionParams &credentials);
            bool is_connected() override;
            void disconnect() override;
            void reconnect() override;
            bool insert(const AccountRecord &account) override;
            bool update(const AccountRecord &account) override;
            bool drop(const AccountRecord &account) override;
            AccountRecord getAccount(const std::string &username, const std::string &password) override;
            bool insert(const ProfileRecord &profile) override;
            bool update(const ProfileRecord &profile) override;
            bool drop(const ProfileRecord &profileID) override;
            ProfileRecord getProfile(const unsigned int &profileID) override;
            std::vector<ProfileRecord> getProfiles(const unsigned int &accountID) override;
            bool insert(const StockRecord &stock) override;
            bool update(const StockRecord &stock) override;
            bool drop(const StockRecord &stockID) override;
            StockRecord getStock(const std::string &symbol, const std::string &currency) override;
            StockRecord getStock(const unsigned int &stockID) override;
            bool insert(const EquityRecord &equity) override;
            bool update(const EquityRecord &equity) override;
            bool drop(const EquityRecord &equityID) override;
            EquityRecord getEquity(const unsigned int &equityID) override;
            std::vector<EquityRecord> getEquities(const unsigned int &profileID) override;
            ~MySQLClient() = default;
        };
    }; // MYsqlClient
}; // Mysql namespace

#endif