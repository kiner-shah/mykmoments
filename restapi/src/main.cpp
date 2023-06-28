#include "db_utils.h"

#include <iostream>
#include <string>
#include <tuple>
#include <regex>
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
    std::regex bearer_scheme_regex("Bearer\\s+([A-Za-z0-9_\\-.~+]+[=]*)");
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
        CROW_LOG_INFO << "Sending 20 response";
        crow::json::wvalue resp_json{ {"total_moments", 20} };
        return crow::response(crow::status::OK, resp_json); });

    CROW_ROUTE(app, "/addmoment")
        .methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                            { return crow::response(crow::status::OK); });

    CROW_ROUTE(app, "/addmoment")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                         {
        std::string username;
        if (!verify_authorization_header(req, username))
        {
            return crow::response(crow::status::UNAUTHORIZED, mkm::error_str(mkm::ErrorCode::AUTHENTICATION_ERROR));
        }

        crow::multipart::message multi_part_message(req);

        // Check if required fields are there
        std::array<const char*, 3> required_parts{"moment-title", "moment-description", "moment-date"};
        for (size_t i = 0; i < required_parts.size(); i++)
        {
            auto it = multi_part_message.part_map.find(required_parts[i]);
            if (it == multi_part_message.part_map.end())
            {
                std::stringstream message;
                message << required_parts[i] << " is missing";
                return crow::response(crow::status::BAD_REQUEST, message.str());
            }
            if (it->second.body.empty())
            {
                std::stringstream message;
                message << required_parts[i] << " cannot be empty";
                return crow::response(crow::status::BAD_REQUEST, message.str());
            }
        }
        // Check if any optional fields are present
        auto it = multi_part_message.part_map.find("moment-image");
        // Search for image field, if not found return error
        // If image is found, caption should be there else error
        it = multi_part_message.part_map.find("moment-image-caption");
        it = multi_part_message.part_map.find("moment-feelings");


        return crow::response(crow::status::OK, "OK"); });

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

    CROW_ROUTE(app, "/createaccount")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                            {
        crow::multipart::message multi_part_message(req);
        auto get_part_if_present = [&multi_part_message](const char* part_name, std::string& part_value) -> bool
        {
            auto it = multi_part_message.part_map.find(part_name);
            if (it == multi_part_message.part_map.end() || it->second.body.empty())
            {
                return false;
            }
            part_value = it->second.body;
            return true;
        };
        std::string full_name, birth_date, email_id, username, password;
        // Check if required fields are there
        if (!get_part_if_present("fullname", full_name))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'fullname' is missing or empty");
        }
        if (!get_part_if_present("birthdate", birth_date))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'birthdate' is missing or empty");
        }
        if (!get_part_if_present("emailid", email_id))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'emailid' is missing or empty");
        }
        if (!get_part_if_present("username", username))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'username' is missing or empty");
        }
        if (!get_part_if_present("password", password))
        {
            return crow::response(crow::status::BAD_REQUEST, "Required part 'password' is missing or empty");
        }
        if (!mkm::create_new_account(full_name, birth_date, email_id, username, password))
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
        return crow::response(crow::status::OK, resp); });

    CROW_ROUTE(app, "/logout")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                         {
        // Check whether username exists and whether logged in
        // Change status to logged out

        CROW_LOG_INFO << "Logged out";
        return crow::response(crow::status::OK, "Logged out"); });

    app.loglevel(crow::LogLevel::Debug);

    // set the port, set the app to run on multiple threads, and run the app
    app.bindaddr("127.0.0.1").port(5000).run();
}