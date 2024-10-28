#pragma once
#include <gmock/gmock.h>
#include "databaseClient.hpp"

class DatabaseClientMock : public database::DatabaseClient
{
public:
    MOCK_METHOD(bool, is_connected, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(void, reconnect, (), (override));
    MOCK_METHOD(bool, insert, (const database::AccountRecord &account), (override));
    MOCK_METHOD(bool, update, (const database::AccountRecord &account), (override));
    MOCK_METHOD(bool, drop, (const database::AccountRecord &account), (override));
    MOCK_METHOD(database::AccountRecord, getAccount, ((const std::string &username), (const std::string &password)), (override));
    MOCK_METHOD(bool, insert, (const database::ProfileRecord &), (override));
    MOCK_METHOD(bool, update, (const database::ProfileRecord &), (override));
    MOCK_METHOD(bool, drop, (const database::ProfileRecord &), (override));
    MOCK_METHOD(database::ProfileRecord, getProfile, (const unsigned int &profileID), (override));
    MOCK_METHOD(std::vector<database::ProfileRecord>, getProfiles, (const unsigned int &accountID), (override));
    MOCK_METHOD(bool, insert, (const database::StockRecord &stock), (override));
    MOCK_METHOD(bool, update, (const database::StockRecord &stock), (override));
    MOCK_METHOD(bool, drop, (const database::StockRecord &stock), (override));
    MOCK_METHOD(database::StockRecord, getStock, ((const std::string &symbol), (const std::string &currency)), (override));
    MOCK_METHOD(database::StockRecord, getStock, (const unsigned int &stockID), (override));
    MOCK_METHOD(bool, insert, (const database::EquityRecord &equity), (override));
    MOCK_METHOD(bool, update, (const database::EquityRecord &equity), (override));
    MOCK_METHOD(bool, drop, (const database::EquityRecord &equity), (override));
    MOCK_METHOD(database::EquityRecord, getEquity, (const unsigned int &equityID), (override));
    MOCK_METHOD(std::vector<database::EquityRecord>, getEquities, (const unsigned int &profileID), (override));
};