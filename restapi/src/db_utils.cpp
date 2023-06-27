#include "db_utils.h"

#include <crow/logging.h>
#include <pqxx/pqxx>

namespace mkm
{
    std::variant<User, ErrorCode> get_user_details(const std::string &username)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm user=user1 password=test123");

        pqxx::read_transaction transaction(c);

        try
        {
            auto row = transaction.exec_params1("SELECT * FROM MKM.USERS WHERE USERNAME=%s", transaction.quote(username));
            transaction.commit();
            return User{
                .username = row["username"].c_str(),
                .password_hash = row["password_hash"].c_str()
            };
        }
        catch (const pqxx::unexpected_rows &e)
        {
            CROW_LOG_ERROR << "Number of rows returned is not equal to 1: " << e.what();
            return ErrorCode::INTERNAL_ERROR;
        }
    }

    bool is_password_valid(const std::string &input_password, const std::string &stored_password_hash)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm user=user1 password=test123");

        pqxx::read_transaction transaction(c);

        try
        {
            // References:
            // [1] https://stackoverflow.com/a/18687445
            // [2] https://www.postgresql.org/docs/current/pgcrypto.html

            std::stringstream s;
            s << "SELECT (CASE WHEN crypt(" << transaction.quote(input_password) << ',' << transaction.quote(stored_password_hash)
              << ") = " << transaction.quote(stored_password_hash) << " then true else false end) as is_equal";
            const std::string query = s.str();
            CROW_LOG_DEBUG << "Query: " << query;
            bool is_password_match = transaction.query_value<bool>(query);
            transaction.commit();
            return is_password_match;
        }
        catch (const pqxx::unexpected_rows &e)
        {
            CROW_LOG_ERROR << "Number of rows returned is not equal to 1: " << e.what();
            return false;
        }
        catch (const pqxx::usage_error &e)
        {
            CROW_LOG_ERROR << "Number of columns returned is not equal to 1: " << e.what();
            return false;
        }
    }
} // namespace mkm