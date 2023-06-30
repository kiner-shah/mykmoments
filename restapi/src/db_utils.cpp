#include "db_utils.h"

#include <crow/logging.h>
#include <pqxx/pqxx>
#include <cstddef>

namespace mkm
{
    std::variant<User, ErrorCode> get_user_details(const std::string &username)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm_db user=mkm_user password=<PASSWORD>");

        pqxx::read_transaction transaction(c);

        try
        {
            auto row = transaction.exec1("SELECT * FROM users WHERE username=" + transaction.quote(username));
            transaction.commit();
            return User{
                .username = row["username"].c_str(),
                .password_hash = row["password_hash"].c_str(),
                .full_name = row["fullname"].c_str(),
                .birth_date = row["birthdate"].c_str(),
                .email_id = row["emailid"].c_str(),
                .account_creation_date = row["account_creation_time"].c_str()
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
        pqxx::connection c("dbname=mkm_db user=mkm_user password=<PASSWORD>");

        pqxx::read_transaction transaction(c);

        try
        {
            // References:
            // [1] https://stackoverflow.com/a/18687445
            // [2] https://www.postgresql.org/docs/current/pgcrypto.html

            std::stringstream s;
            s << "SELECT (CASE WHEN crypt(" << transaction.quote(input_password) << ',' << transaction.quote(stored_password_hash)
              << ") = " << transaction.quote(stored_password_hash) << " THEN true ELSE false END) as is_equal";
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
        catch(const pqxx::sql_error& e)
        {
            CROW_LOG_ERROR << "Internal exception was thrown: " << e.what();
            return false;
        }
    }

    bool create_new_account(const User& user_details, const std::string& password)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm_db user=mkm_user password=<PASSWORD>");

        pqxx::work transaction(c);

        try
        {
            std::stringstream s;
            s << "INSERT INTO users(username, fullname, birthdate, emailid, password_hash, account_creation_time) "
                "VALUES("
                << transaction.quote(user_details.username) <<  ','
                << transaction.quote(user_details.full_name) << ','
                << transaction.quote(user_details.birth_date) << ','
                << transaction.quote(user_details.email_id) << ", crypt("
                << transaction.quote(password) << ", gen_salt('bf', 8)), now())";
            const std::string query = s.str();
            CROW_LOG_DEBUG << "Query: " << query;
            auto result = transaction.exec(query);
            if (result.affected_rows() != 1)
            {
                CROW_LOG_ERROR << "Something went wrong - couldn't insert data into database table";
                return false;
            }
            transaction.commit();
            return true;
        }
        catch(const pqxx::sql_error& e)
        {
            CROW_LOG_ERROR << "Internal exception was thrown: " << e.what();
            return false;
        }
    }

    bool add_new_moment(const Moment& moment)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm_db user=mkm_user password=<PASSWORD>");

        pqxx::work transaction(c);

        try
        {
            std::string image_content_bytes{ transaction.quote_raw(
                std::basic_string_view<std::byte>{
                    reinterpret_cast<std::byte const*>(std::data(moment.image_content)),
                    std::size(moment.image_content)
                }
            )};

            // Will use a SQL trigger / function which will get triggered before insert
            // new_id = select count(*) + 1 from moments where username=<username>
            // https://stackoverflow.com/q/61917751
            std::stringstream s;
            s << "INSERT INTO moments(username, title, description, moment_date, image_filename, image_data, image_caption, feelings) "
                "VALUES("
                << transaction.quote(moment.username) <<  ','
                << transaction.quote(moment.title) << ','
                << transaction.quote(moment.description) << ','
                << transaction.quote(moment.date) << ','
                << transaction.quote(moment.image_filename) << ','
                << image_content_bytes << ','
                << transaction.quote(moment.image_caption) << ", '{";
            for (size_t i = 0; i < moment.feelings.size(); i++)
            {
                s << '"' << transaction.esc(moment.feelings[i]) << '"';
                if (i != moment.feelings.size() - 1)
                {
                    s << ',';
                }
            }
            s << "}')";

            auto result = transaction.exec(s.str());
            if (result.affected_rows() != 1)
            {
                CROW_LOG_ERROR << "Something went wrong - couldn't insert data into database table";
                return false;
            }
            transaction.commit();
            return true;
        }
        catch(const pqxx::sql_error& e)
        {
            CROW_LOG_ERROR << "Internal exception was thrown: " << e.what();
            return false;
        }
    }

    uint64_t get_moment_count(const std::string& username)
    {
        // TODO: replace hardcoded string with values from some environment/properties file
        pqxx::connection c("dbname=mkm_db user=mkm_user password=<PASSWORD>");

        pqxx::read_transaction transaction(c);

        try
        {
            auto row = transaction.exec1("SELECT COUNT(*) as total_moments FROM moments WHERE username=" + transaction.quote(username));
            transaction.commit();
            return row["total_moments"].as<uint64_t>();
        }
        catch (const pqxx::unexpected_rows &e)
        {
            CROW_LOG_ERROR << "Number of rows returned is not equal to 1: " << e.what();
            return 0;
        }
    }
} // namespace mkm