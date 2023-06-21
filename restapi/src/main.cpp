#include <iostream>
#include <string>
#include "crow.h"
#include "crow/middlewares/cors.h"
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>

int main()
{
    crow::App<crow::CORSHandler> app; //define your crow application

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

    CROW_ROUTE(app, "/addmoment")
        .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        crow::multipart::message multi_part_message(req);

        // Check if required fields are there
        std::array<const char*, 5> required_parts{"moment-title", "moment-description", "moment-date", "moment-image", "moment-image-caption"};
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
        auto it = multi_part_message.part_map.find("moment-feelings");


        return crow::response(crow::status::OK, "OK");
    });

    CROW_ROUTE(app, "/login")
        .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
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
        // Check whether database has username, if not return 401, else retrive salt value
        // Compute hash of (password + salt)
        // Check if hash value matches with that in database, if not return 401
        // Update login status, create a signed JWT token, return it in the response
        auto current_time = std::chrono::system_clock::now();
        auto token = jwt::create()
                    .set_issuer("MKM000100")
                    .set_type("JWS")
                    .set_issued_at(current_time)
                    .set_expires_at(current_time + std::chrono::seconds{60})    // TODO: change it later to appropriate expiry
                    .set_payload_claim("username", jwt::claim(std::string("Kiner Shah")))
                    .sign(jwt::algorithm::hs512{"secret"});
        
        crow::json::wvalue resp{ {"access_token", token}, {"username", "Kiner Shah"} };
        return crow::response(crow::status::OK, resp);
    });

    CROW_ROUTE(app, "/logout")
        .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        // Check whether username exists and whether logged in
        // Change status to logged out

        CROW_LOG_INFO << "Logged out";
        return crow::response(crow::status::OK, "Logged out");
    });

    app.loglevel(crow::LogLevel::Info);

    //set the port, set the app to run on multiple threads, and run the app
    app.bindaddr("127.0.0.1").port(5000).run();
}