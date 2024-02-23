#include <iostream>
#include "json.hpp"
#include <fstream>
#include "HTTPRequest.hpp"
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using JSON = nlohmann::json;

void printReport(JSON response) {
    cout << "Отчет про: " << response["email"] << endl;
    cout << "Формат правильный: " << response["is_valid_format"]["value"] << endl;
    cout << "Одноразовый: " << response["is_disposable_email"]["value"] << endl;
    cout << "MX найден: " << response["is_mx_found"]["value"] << endl;
    cout << "SMTP правильный: " << response["is_smtp_valid"]["value"] << endl;
    cout << "- - - - -" << endl << endl;
}

int main() {
    ifstream f("../mail.json");
    JSON data = JSON::parse(f);
    for (auto d : data) {
        try {
            string key = "API_KEY";
            string url = "http://emailvalidation.abstractapi.com/v1/?api_key=" + key + "&email=" + d.get<string>();
            http::Request request{url};
            const auto response = request.send("GET");
            JSON mainResponse = JSON::parse(string{response.body.begin(), response.body.end()});
            printReport(mainResponse);
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
        catch (const exception &e) {
            cerr << "Request failed, error: " << e.what() << endl;
        }
    }
    return 0;
}