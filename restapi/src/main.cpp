#include <iostream>
#include "crow.h"
#include <pqxx/pqxx>

int main()
{
    crow::SimpleApp app; //define your crow application

    // Connect to the database.  You can have multiple connections open
    // at the same time, even to the same database.
    pqxx::connection c("dbname=mydb user=user1 password=test123");
    std::cout << "Connected to server with version " << c.server_version() << '\n';

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([]() {
        
        return "Hello world";
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}