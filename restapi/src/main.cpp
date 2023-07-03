#include "db_utils.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <regex>
#include <optional>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>

struct RequestLogger
{
    struct context
    {
    };

    // This method is run before handling the request
    void before_handle(crow::request &req, crow::response & /*res*/, context & /*ctx*/)
    {
        CROW_LOG_DEBUG << "Before request handle: " + req.url;
    }

    // This method is run after handling the request
    void after_handle(crow::request &req, crow::response & /*res*/, context & /*ctx*/)
    {
        CROW_LOG_DEBUG << "After request handle: " << req.url;
    }
};
/**
 * @brief Verify the authorization header containing the bearer token and
 * extract username from the token payload.
 *
 * @param[in] req The request object
 * @param[out] username The string to which extracted username will be written to
 * @retval true Successful verification and @p username will be set.
 * @retval false Verification failed and @p username will be empty.
 */
static bool verify_authorization_header(const crow::request &req, std::string &username)
{
    const auto &headers_it = req.headers.find("Authorization");
    if (headers_it == req.headers.end())
    {
        CROW_LOG_ERROR << "Request headers doesn't contain Authorization";
        return false;
    }
    const std::string &authorization_value = headers_it->second;
    // Authorization header value (Bearer token) should be of format as per
    // [RFC 6750](https://datatracker.ietf.org/doc/html/rfc6750#section-2.1)
    std::regex bearer_scheme_regex("Bearer +([A-Za-z0-9_\\-.~+]+[=]*)");
    std::smatch m;
    if (!std::regex_match(authorization_value, m, bearer_scheme_regex))
    {
        CROW_LOG_ERROR << "Authorization header's value has invalid format";
        return false;
    }

    auto decoded_token = jwt::decode(m[1].str());
    // Decode base64 encoded JWT, verify signature, check expiry
    auto verifier = jwt::verify()
                        .allow_algorithm(jwt::algorithm::hs512{"secret"})
                        .with_issuer("MKM");
    try
    {
        verifier.verify(decoded_token);
    }
    catch(const std::exception& e)
    {
        CROW_LOG_ERROR << "Token verification error: " << e.what();
        return false;
    }

    // Set username from value retrieved from JWT payload
    username = decoded_token.get_payload_claim("username").as_string();
    return true;
}
/**
 * @brief Get the value (string) of a part from a multi-part request if present
 *
 * @param[in] multi_part_message The multi-part request object
 * @param[in] part_name The name of the part whose value needs to be retrieved
 * @param[out] part_value The string to which the value will be stored
 * @retval true If part is present, it's value is non-empty. @p part_value will be set
 * @retval false If part is not present or present but empty. @p part_value will not be set
 */
static bool get_part_value_string_if_present(const crow::multipart::message& multi_part_message, const char* part_name, std::string& part_value)
{
    auto it = multi_part_message.part_map.find(part_name);
    if (it == multi_part_message.part_map.end() || it->second.body.empty())
    {
        return false;
    }
    part_value = it->second.body;
    return true;
};

/**
 * @brief Get the value (file name + file contents) of a part from a multi-part request if present
 *
 * @param[in] multi_part_message The multi-part request object
 * @param[in] part_name The name of the part whose value needs to be retrieved
 * @param[out] file_name The file name of the uploaded file
 * @param[out] file_contents The contents of the uploaded file
 * @retval true If part is present, it's value is non-empty. @p file_name will be set
 * @retval false If part is not present or present but empty. @p file_name will not be set
 */
static bool create_file_from_part_value_if_present(const crow::multipart::message& multi_part_message, const char* part_name, std::string& file_name, std::vector<std::byte>& file_contents)
{
    auto it = multi_part_message.part_map.find(part_name);
    if (it == multi_part_message.part_map.end() || it->second.body.empty())
    {
        return false;
    }
    // Extract the file name
    auto headers_it = it->second.headers.find("Content-Disposition");
    if (headers_it == it->second.headers.end())
    {
        CROW_LOG_ERROR << "No Content-Disposition found";
        return false;
    }
    auto params_it = headers_it->second.params.find("filename");
    if (params_it == headers_it->second.params.end())
    {
        CROW_LOG_ERROR << "Part with name '" << part_name << "' should have a file";
        return false;
    }

    file_name = params_it->second;
    std::transform(it->second.body.begin(), it->second.body.end(), std::back_inserter(file_contents), [](char c) { return std::byte(c); });
    return true;
};

int main()
{
    crow::App<RequestLogger, crow::CORSHandler> app; // define your crow application

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    // clang-format off
    cors
      .global()
        .methods("POST"_method, "GET"_method)
      .prefix("/")
        .origin("http://localhost:8080")
        .allow_credentials();
    // clang-format on

    // Connect to the database.  You can have multiple connections open
    // at the same time, even to the same database.
    // pqxx::connection c("dbname=mydb user=user1 password=test123");
    // std::cout << "Connected to server with version " << c.server_version() << '\n';

    CROW_ROUTE(app, "/gettotalmoments")
        .methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                            { return crow::response(crow::status::OK); });

    CROW_ROUTE(app, "/gettotalmoments")
        .methods(crow::HTTPMethod::GET)([](const crow::request &req)
                                        {
        std::string username;
        if (!verify_authorization_header(req, username))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }

        crow::json::wvalue resp_json{ {"total_moments", mkm::get_moment_count(username)} };
        return crow::response(crow::status::OK, resp_json);
    });

    CROW_ROUTE(app, "/addmoment")
        .methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                            { return crow::response(crow::status::OK); });

    CROW_ROUTE(app, "/addmoment")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                         {
        mkm::Moment moment;
        if (!verify_authorization_header(req, moment.username))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }

        crow::multipart::message multi_part_message(req);

        // Check if required fields are there
        if (!get_part_value_string_if_present(multi_part_message, "moment-title", moment.title))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'moment-title' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "moment-description", moment.description))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'moment-description' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "moment-date", moment.date))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'moment-date' is missing or empty");
        }

        // Check if any optional fields are present
        if (create_file_from_part_value_if_present(multi_part_message, "moment-image", moment.image_filename, moment.image_content))
        {
            if (!get_part_value_string_if_present(multi_part_message, "moment-image-caption", moment.image_caption))
            {
                return crow::response(crow::status::BAD_REQUEST, "Required part 'moment-image-caption' is missing or empty");
            }
        }
        std::string moment_feelings_str;
        if (get_part_value_string_if_present(multi_part_message, "moment-feelings", moment_feelings_str))
        {
            std::stringstream s(moment_feelings_str);
            std::string feeling;
            while (std::getline(s, feeling, ','))
            {
                moment.feelings.emplace_back(std::move(feeling));
            }
        }
        if (!mkm::add_new_moment(moment))
        {
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, mkm::error_str(mkm::ErrorCode::INTERNAL_ERROR));
        }
        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/getuserdetails")
        .methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                            { return crow::response(crow::status::OK); });

    CROW_ROUTE(app, "/getuserdetails")
        .methods(crow::HTTPMethod::GET)([](const crow::request &req)
                                         {
        std::string username;
        if (!verify_authorization_header(req, username))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }

        auto result = mkm::get_user_details(username);
        if (std::holds_alternative<mkm::ErrorCode>(result))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(std::get<mkm::ErrorCode>(result)));
        }
        const auto& user = std::get<mkm::User>(result);

        crow::json::wvalue resp_json{
            {"username", user.username},
            {"fullname", user.full_name},
            {"birthdate", user.birth_date},
            {"emailid", user.email_id}
        };
        return crow::response(crow::status::OK, resp_json);
    });

    CROW_ROUTE(app, "/getmomentlist")
        .methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                            { return crow::response(crow::status::OK); });
    
    CROW_ROUTE(app, "/getmomentlist")
        .methods(crow::HTTPMethod::GET)([](const crow::request &req)
                                            {
        std::string username;
        if (!verify_authorization_header(req, username))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }
        const crow::query_string& qs = req.url_params;
        uint32_t page_size = std::stoi(qs.get("page_size"));
        uint64_t current_page = std::stoi(qs.get("current_page"));
        std::optional<std::string> search = (qs.get("search") == nullptr ? std::nullopt : std::make_optional(qs.get("search")));
        std::optional<std::string> sort_by = (qs.get("sort_by") == nullptr ? std::nullopt : std::make_optional(qs.get("sort_by")));

        auto result = mkm::get_moments_list(username, page_size, current_page, std::move(sort_by), std::move(search));
        if (std::holds_alternative<mkm::ErrorCode>(result))
        {
            mkm::ErrorCode ec = std::get<mkm::ErrorCode>(result);
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, mkm::error_str(ec));
        }
        const auto& moments = std::get< std::vector<mkm::Moment> >(result);
        std::vector<crow::json::wvalue> moments_json_list;
        for (const auto& moment : moments)
        {
            // std::vector<crow::json::wvalue> image_content_bytes;
            std::stringstream s;
            for (auto x : moment.image_content)
            {
                // image_content_bytes.push_back(std::to_integer<uint8_t>(x));
                s << std::setw(2) << std::setfill('0') << std::hex << std::to_integer<uint16_t>(x);
            }

            std::string image_content_hex = s.str();
            
            crow::json::wvalue entry{
                {"id", moment.id},
                {"username", moment.username},
                {"title", moment.title},
                {"short_description", moment.description.substr(0, 200)},
                {"image_filename", moment.image_filename},
                {"image_data", image_content_hex},
                {"created_time", moment.created_date},
                {"last_modified_time", moment.last_modified_date}
            };
            moments_json_list.push_back(entry);
        }
        return crow::response(crow::status::OK, crow::json::wvalue({ {"moments", moments_json_list} }));
    });

    CROW_ROUTE(app, "/createaccount")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                            {
        crow::multipart::message multi_part_message(req);

        mkm::User user_details;
        std::string password;
        // Check if required fields are there
        if (!get_part_value_string_if_present(multi_part_message, "fullname", user_details.full_name))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'fullname' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "birthdate", user_details.birth_date))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'birthdate' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "emailid", user_details.email_id))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'emailid' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "username", user_details.username))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'username' is missing or empty");
        }
        if (!get_part_value_string_if_present(multi_part_message, "password", password))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'password' is missing or empty");
        }
        if (!mkm::create_new_account(user_details, password))
        {
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, mkm::error_str(mkm::ErrorCode::INTERNAL_ERROR));
        }

        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/login")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                         {
        // Request body loading and validation
        const auto& headers_it = req.headers.find("Content-Length");
        if (headers_it == req.headers.end())
        {
            return crow::response(crow::status::BAD_REQUEST, "Content-Length header is missing");
        }
        int request_body_length = std::stoi(headers_it->second);
        
        // "username" and "password" along with '{', '}', comma and quotes itself leads to 23 characters
        if (request_body_length <= 23)
        {
            return crow::response(crow::status::BAD_REQUEST, "Request body is too short");
        }
        crow::json::rvalue request_body_json = crow::json::load(req.body.c_str(), request_body_length);
        if (request_body_json.error())
        {
            return crow::response(crow::status::BAD_REQUEST, "Invalid request body - should be a valid JSON string");
        }

        // Check whether request body has username and password, if not return 400
        if (!request_body_json.has("username") || !request_body_json.has("password") 
            || request_body_json["username"].s().size() == 0 || request_body_json["password"].s().size() == 0)
        {
            return crow::response(crow::status::BAD_REQUEST, "Invalid request body - missing username and/or password");
        }
        const std::string username = request_body_json["username"].s();
        const std::string password = request_body_json["password"].s();

        // Check whether database has username, if not return 401, else retrive salt value
        auto result = mkm::get_user_details(username);
        if (std::holds_alternative<mkm::ErrorCode>(result))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(std::get<mkm::ErrorCode>(result)));
        }
        const auto& user = std::get<mkm::User>(result);

        // Compute hash of input password
        // Check if hash value matches with that in database, if not return 401
        if (!mkm::is_password_valid(password, user.password_hash))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }

        // Update login status, create a signed JWT token, return it in the response
        auto current_time = std::chrono::system_clock::now();
        auto token = jwt::create()
                    .set_issuer("MKM")
                    .set_type("JWS")
                    .set_issued_at(current_time)
                    .set_expires_at(current_time + std::chrono::seconds{3600})    // TODO: change it later to appropriate expiry
                    .set_payload_claim("username", jwt::claim(user.username))
                    .sign(jwt::algorithm::hs512{"secret"});
        
        crow::json::wvalue resp{ {"access_token", token}, {"username", user.username} };
        return crow::response(crow::status::OK, resp);
    });

    app.loglevel(crow::LogLevel::Debug);

    // set the port, set the app to run on multiple threads, and run the app
    app.bindaddr("127.0.0.1").port(5000).run();
}