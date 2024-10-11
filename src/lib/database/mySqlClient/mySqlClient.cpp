#include "mySqlClient.hpp"
#include <vector>
#include <algorithm>

const int MAX_PROFILES = 5;
const int MAX_EQUITIES = 20;

const std::string CreateAccountsTableQuery("CREATE TABLE IF NOT EXISTS accounts ("
                                           "`ID` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,"
                                           "`username` VARCHAR(45) NOT NULL,"
                                           "`password` VARCHAR(45) NOT NULL,"
                                           "PRIMARY KEY(`ID`),"
                                           "UNIQUE INDEX `username_UNIQUE` (`username` ASC)VISIBLE,"
                                           "UNIQUE INDEX `password_UNIQUE` (`password` ASC)VISIBLE)");

const std::string CreateProfilesTableQuery("CREATE TABLE IF NOT EXISTS profiles ("
                                           "`ID` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,"
                                           "`account` INT UNSIGNED ZEROFILL NOT NULL,"
                                           "`name` VARCHAR(45) NOT NULL DEFAULT \"No Name\","
                                           "`currency` VARCHAR(45) NOT NULL DEFAULT \"USD\","
                                           "`initialValue` FLOAT NOT NULL DEFAULT 1000,"
                                           "PRIMARY KEY(`ID`),"
                                           "INDEX `account_idx` (`account` ASC)VISIBLE,"
                                           "CONSTRAINT `account` FOREIGN KEY(`account`)"
                                           "REFERENCES `accounts` (`ID`) "
                                           "ON DELETE NO ACTION "
                                           "ON UPDATE NO ACTION)");

const std::string CreateStocksTableQuery("CREATE TABLE IF NOT EXISTS stocks ("
                                         "`ID` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,"
                                         "`name` VARCHAR(45) NOT NULL,"
                                         "`symbol` VARCHAR(45) NOT NULL,"
                                         "`currency` VARCHAR(45) NOT NULL DEFAULT \"USD\","
                                         "`currentPrice` DECIMAL(12, 6) NOT NULL,"
                                         "`lastUpdate` DATE NOT NULL,"
                                         "PRIMARY KEY(`ID`))");

const std::string CreateEquitiesTableQuery("CREATE TABLE IF NOT EXISTS equities ("
                                           "`ID` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,"
                                           "`profile` INT UNSIGNED ZEROFILL NOT NULL,"
                                           "`stock` INT UNSIGNED ZEROFILL NOT NULL,"
                                           "`quantity` INT NOT NULL,"
                                           "`purchasePrice` DECIMAL(12, 6) NOT NULL,"
                                           "PRIMARY KEY(`ID`),"
                                           "INDEX `profile_idx` (`profile` ASC) VISIBLE,"
                                           "INDEX `stock_idx` (`stock` ASC) VISIBLE,"
                                           "CONSTRAINT `profile` FOREIGN KEY(`profile`)"
                                           "REFERENCES `profiles` (`ID`) "
                                           "ON DELETE NO ACTION "
                                           "ON UPDATE NO ACTION,"
                                           "CONSTRAINT `stock` FOREIGN KEY(`stock`)"
                                           "REFERENCES `stocks` (`ID`)"
                                           "ON DELETE NO ACTION "
                                           "ON UPDATE NO ACTION)");

const std::string CreateDeleteRelatedProfilesAndEquitiesTrigger("CREATE TRIGGER IF NOT EXISTS `DeleteRelatedProfilesAndEquities` "
                                                                "BEFORE DELETE ON `accounts` FOR EACH ROW BEGIN "
                                                                "DELETE FROM equities "
                                                                "WHERE profile = ANY(SELECT account "
                                                                "FROM profiles WHERE account = OLD.ID); "
                                                                "DELETE FROM profiles WHERE account = OLD.ID; "
                                                                "END");

const std::string CreateDeleteRelatedEquitiesTrigger("CREATE TRIGGER IF NOT EXISTS `DeleteRelatedEquities` "
                                                     "BEFORE DELETE ON `profiles` FOR EACH ROW BEGIN "
                                                     "DELETE FROM equities WHERE profile = OLD.ID; "
                                                     "END");

bool database::mysql::MySQLClient::inputQuery(const std::string &query)
{
    try
    {
        sql << query;
        return true;
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

template <typename T>
T database::mysql::MySQLClient::singleOutputQuery(std::string query)
{
    T record;
    soci::indicator ind;
    try
    {
        sql << query, soci::into(record, ind);
        if (!sql.got_data() || ind != soci::i_ok)
            record.reset();
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
        record.reset();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        record.reset();
    }
    return record;
}

template <typename T>
std::vector<T> database::mysql::MySQLClient::multipleOutputQuery(std::string query)
{
    std::vector<T> records;

    /*for (auto &row : rs)
        records.push_back(row);*/
    try
    {
        soci::rowset<T> rs = sql.prepare << query;
        std::copy(rs.begin(), rs.end(), std::back_inserter(records));
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
        records.clear();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        records.clear();
    }
    return records;
}

database::mysql::MySQLClient::MySQLClient(const connectionParams &credentials) : credentials_(credentials)
{
    soci::connection_parameters parameters(soci::mysql, "db=" + credentials_.database + " user=" + credentials_.user + " password=" + credentials_.password + " reconnect=1");
    try
    {
        sql.open(parameters);
        sql << CreateAccountsTableQuery;
        sql << CreateProfilesTableQuery;
        sql << CreateStocksTableQuery;
        sql << CreateEquitiesTableQuery;
        sql << CreateDeleteRelatedProfilesAndEquitiesTrigger;
        sql << CreateDeleteRelatedEquitiesTrigger;
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void database::mysql::MySQLClient::reset(const connectionParams &credentials)
{
    credentials_ = credentials;
    soci::connection_parameters parameters(soci::mysql, "db=" + credentials_.database + " user=" + credentials_.user + " password=" + credentials_.password + " reconnect=1");
    try
    {
        sql.close();
        sql.open(parameters);
        sql << CreateAccountsTableQuery;
        sql << CreateProfilesTableQuery;
        sql << CreateStocksTableQuery;
        sql << CreateEquitiesTableQuery;
        sql << CreateDeleteRelatedProfilesAndEquitiesTrigger;
        sql << CreateDeleteRelatedEquitiesTrigger;
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

bool database::mysql::MySQLClient::is_connected()
{
    try
    {
        return sql.is_connected();
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

void database::mysql::MySQLClient::disconnect()
{
    try
    {
        sql.close();
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void database::mysql::MySQLClient::reconnect()
{
    try
    {
        sql.reconnect();
    }
    catch (soci::mysql_soci_error const &e)
    {
        std::cerr << "MySQL error: " << e.err_num_
                  << " " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

bool database::mysql::MySQLClient::insert(const AccountRecord &account)
{
    std::string query = "INSERT INTO accounts(username,password)";
    query += " VALUES(\"" + account.username + "\", \"" + account.password + "\")";
    return inputQuery(query);
}

bool database::mysql::MySQLClient::update(const AccountRecord &account)
{
    std::string query = "UPDATE accounts SET username = \"" + account.username + "\", password = \"" + account.password + "\"";
    query += " WHERE ID = " + std::to_string(account.ID);
    return inputQuery(query);
}

bool database::mysql::MySQLClient::drop(const AccountRecord &account)
{
    std::string query = "DELETE FROM accounts WHERE ID = " + std::to_string(account.ID);
    return inputQuery(query);
}

database::AccountRecord database::mysql::MySQLClient::getAccount(const std::string &username, const std::string &password)
{
    // FIXME: query
    std::string query = "SELECT * FROM accounts WHERE username = \"" + username + "\" AND password = \"" + password + "\"";
    return singleOutputQuery<AccountRecord>(query);
}

bool database::mysql::MySQLClient::insert(const ProfileRecord &profile)
{
    std::string query = "INSERT INTO profiles(account, name, currency, initialValue)";
    query += " VALUES(" + std::to_string(profile.accountID) + ", \"" + profile.name + "\", \"" + profile.currency + "\", " + std::to_string(profile.initialValue) + ")";
    return inputQuery(query);
}

bool database::mysql::MySQLClient::update(const ProfileRecord &profile)
{
    std::string query = "UPDATE profiles SET name = \"" + profile.name + "\" ";
    query += " WHERE account = \"" + std::to_string(profile.accountID) + "\"";
    return inputQuery(query);
}

bool database::mysql::MySQLClient::drop(const ProfileRecord &profile)
{
    std::string query = "DELETE FROM profiles WHERE ID = " + std::to_string(profile.ID);
    return inputQuery(query);
}

std::vector<database::ProfileRecord> database::mysql::MySQLClient::getProfiles(const unsigned int &accountID)
{
    // FIXME: query
    std::string query = "SELECT * FROM profiles WHERE account = " + std::to_string(accountID);
    return multipleOutputQuery<ProfileRecord>(query);
}

bool database::mysql::MySQLClient::insert(const StockRecord &stock)
{
    std::string query = "INSERT INTO stocks(name, symbol, currency, currentPrice, lastUpdate)";
    query += " VALUES(\"" + stock.name + "\", \"" + stock.symbol + "\", \"" + stock.currency + "\", " + std::to_string(stock.currentPrice) + ", \"" + stock.lastUpdate + "\")";
    return inputQuery(query);
}

bool database::mysql::MySQLClient::update(const StockRecord &stock)
{
    std::string query = "UPDATE stocks SET name = \"" + stock.name + "\", symbol = \"" + stock.symbol + "\", ";
    query += "currency = \"" + stock.currency + "\", currentPrice = " + std::to_string(stock.currentPrice) + ", lastUpdate = \"" + stock.lastUpdate + "\" ";
    query += " WHERE ID = " + std::to_string(stock.ID);
    return inputQuery(query);
}

bool database::mysql::MySQLClient::drop(const StockRecord &stock)
{
    std::string query = "DELETE FROM stocks WHERE ID = " + std::to_string(stock.ID);
    return inputQuery(query);
}

database::StockRecord database::mysql::MySQLClient::getStock(const std::string &symbol, const std::string &currency)
{
    // FIXME: query
    std::string query = "SELECT * FROM stocks WHERE symbol = \"" + symbol + "\" AND currency = \"" + currency + "\"";
    return singleOutputQuery<StockRecord>(query);
}

bool database::mysql::MySQLClient::insert(const EquityRecord &equity)
{
    std::string query = "INSERT INTO equities(profile, stock, initialValue, quantity)";
    query += " VALUES(" + std::to_string(equity.profileID) + ", " + std::to_string(equity.stockID) + ", " + std::to_string(equity.initialValue) + ", " + std::to_string(equity.quantity) + ")";
    return inputQuery(query);
}

bool database::mysql::MySQLClient::update(const EquityRecord &equity)
{
    std::string query = "UPDATE equities SET quantity = " + std::to_string(equity.quantity);
    query += " WHERE ID = " + std::to_string(equity.ID);
    return inputQuery(query);
}

bool database::mysql::MySQLClient::drop(const EquityRecord &equity)
{
    std::string query = "DELETE FROM equities WHERE ID = " + std::to_string(equity.ID);
    return inputQuery(query);
}

std::vector<database::EquityRecord> database::mysql::MySQLClient::getEquities(const unsigned int &profileID)
{
    // FIXME: query
    std::string query = "SELECT * FROM equities WHERE profile = " + std::to_string(profileID);
    return multipleOutputQuery<EquityRecord>(query);
}