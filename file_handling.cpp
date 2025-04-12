#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

const string USER_DATA_FILE = "user_data.json";

// Helper function to initialize the user file
void init_user_file()
{
    ifstream file(USER_DATA_FILE);
    if (!file.good())
    {
        ofstream outfile(USER_DATA_FILE);
        json initial_data;
        initial_data["users"] = json::array();
        outfile << initial_data.dump(2);
        outfile.close();
    }
}

// Save user data to file
bool save_user_data(const string &username, const string &hashed_password, const string &salt)
{
    init_user_file();

    // Read existing data
    ifstream infile(USER_DATA_FILE);
    json data;
    try
    {
        infile >> data;
        infile.close();

        // Check if user already exists
        for (const auto &user : data["users"])
        {
            if (user["username"] == username)
            {
                return false;
            }
        }

        // Add new user
        json new_user;
        new_user["username"] = username;
        new_user["hashed_password"] = hashed_password;
        new_user["salt"] = salt;
        data["users"].push_back(new_user);

        // Write back to file
        ofstream outfile(USER_DATA_FILE);
        outfile << data.dump(2);
        outfile.close();
        return true;
    }
    catch (const exception &e)
    {
        cerr << "Error saving user data: " << e.what() << endl;
        return false;
    }
}

// Retrieve user data by username
pair<string, string> get_user_data(const string &username)
{
    ifstream file(USER_DATA_FILE);
    if (!file.good())
    {
        return make_pair("", "");
    }

    try
    {
        json data;
        file >> data;

        for (const auto &user : data["users"])
        {
            if (user["username"] == username)
            {
                return make_pair(
                    user["hashed_password"].get<string>(),
                    user["salt"].get<string>());
            }
        }
    }
    catch (const exception &e)
    {
        cerr << "Error reading user data: " << e.what() << endl;
    }

    return make_pair("", "");
}