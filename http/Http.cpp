#include "lib/json.hpp"
#include "lib/httplib.hpp"
#include "DB.hpp"
#include "Options.hpp"
#include <exception>
#include <memory>

std::unique_ptr<DB> db;

void init()
{
    Options opt;
    db = std::make_unique<DB>(opt);
}

void handleGet(const httplib::Request &req, httplib::Response &res)
{
    if (req.method != "GET")
    {
        res.status = 405; // Method Not Allowed
        res.set_content("Method not allowed", "text/plain");
        return;
    }

    // Extract "key" from query parameters
    auto key = req.get_param_value("key");
    if (key.empty())
    {
        res.status = 400; // Bad Request
        res.set_content("Missing 'key' parameter", "text/plain");
        return;
    }

    std::string value;
    try
    {
        std::cout << "get: " << key << std::endl;
        ;
        value = db->Get(key);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Http::Get Fail! Message: " << e.what() << std::endl;
        res.status = 500; // Internal Server Error
        res.set_content("Failed to get value from DB", "text/plain");
    }

    // Return the value in JSON format
    nlohmann::json json_response = {{"key", key}, {"value", value}};
    res.set_content(json_response.dump(), "application/json");
}

auto handlePut(const httplib::Request &req, httplib::Response &res)
{
    if (req.method != "POST")
    {
        res.status = 405; // Method Not Allowed
        res.set_content("Method not allowed", "text/plain");
        return;
    }

    // Parse the request body as JSON
    nlohmann::json json_data;
    try
    {
        json_data = nlohmann::json::parse(req.body);
    }
    catch (nlohmann::json::parse_error &e)
    {
        res.status = 400; // Bad Request
        res.set_content("Invalid JSON", "text/plain");
        return;
    }

    // Check if it's a map of string to string
    if (!json_data.is_object())
    {
        res.status = 400; // Bad Request
        res.set_content("Expected JSON object", "text/plain");
        return;
    }

    // Iterate over the key-value pairs
    for (auto &[key, value] : json_data.items())
    {
        std::cout << "put: " << key << " : " << value << std::endl;
        if (!value.is_string())
        {
            res.status = 400; // Bad Request
            res.set_content("Expected string values", "text/plain");
            return;
        }
        // Put the key-value pair into the database
        try
        {
            db->Put(key, value);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Http::Put Fail! Message: " << e.what() << std::endl;
            res.status = 500; // Internal Server Error
            res.set_content("Failed to put value in DB", "text/plain");
            return;
        }
    }

    res.status = 200; // OK
    res.set_content("Success", "text/plain");
}

auto handleDelete(const httplib::Request &req, httplib::Response &res)
{
    if (req.method != "DELETE")
    {
        res.status = 405; // Method Not Allowed
        res.set_content("Method not allowed", "text/plain");
        return;
    }

    auto key = req.get_param_value("key");
    if (key.empty())
    {
        res.status = 400; // Bad Request
        res.set_content("Missing 'key' parameter", "text/plain");
        return;
    }

    try
    {
        std::cout << "delete: " << key << std::endl;
        db->Delete(key);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Http::Delete Fail! Message: " << e.what() << std::endl;
        res.status = 500; // Internal Server Error
        res.set_content("Failed to delete value from DB", "text/plain");
    }

    // Return the value in JSON format
    nlohmann::json json_response = {{"key", key}};
    res.set_content(json_response.dump(), "application/json");
}

void handleList(const httplib::Request &req, httplib::Response &res)
{
    if (req.method != "GET")
    {
        res.status = 405; // Method Not Allowed
        res.set_content("Method not allowed", "text/plain");
        return;
    }

    nlohmann::json json_response;
    try
    {
        std::cout << "list" << std::endl;
        json_response = db->ListKey();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Http::List Fail! Message: " << e.what() << std::endl;
        res.status = 500; // Internal Server Error
        res.set_content("Failed to list value from DB", "text/plain");
    }

    // Return the value in JSON format
    res.set_content(json_response.dump(), "application/json");
}

int main()
{
    init();
    httplib::Server server;
    server.Post("/bitcask/put", handlePut);
    server.Get("/bitcask/get", handleGet);
    server.Delete("/bitcask/delete", handleDelete);
    server.Get("/bitcask/list", handleList);
    std::cout << "Server listening on port 8080..." << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}