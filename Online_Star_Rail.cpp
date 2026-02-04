#include <iostream>
#include <fstream>
#include <string>

#include "menu.h"
#include "connect_manager.h"
#include "game.pb.h"

using namespace std;

namespace proto_helpers {
    osr::ServerResponse parseResponse(const std::string& data);
}

static const std::string SESSION_FILE = ".osr_session";

static std::string loadToken() {
    std::ifstream f(SESSION_FILE);
    if (!f.is_open()) return "";
    std::string token;
    std::getline(f, token);
    return token;
}

static void saveToken(const std::string& token) {
    std::ofstream f(SESSION_FILE);
    if (f.is_open()) {
        f << token;
    }
}

static bool authenticate(ConnectManager& conn) {
    // Try saved token first
    std::string token = loadToken();
    if (!token.empty()) {
        std::cout << "Resuming session..." << std::endl;
        conn.sendTokenAuth(token);
        std::string raw = conn.receive();
        osr::ServerResponse resp = proto_helpers::parseResponse(raw);
        if (resp.success()) {
            std::cout << "Session restored (user_id=" << resp.user_id() << ")" << std::endl;
            return true;
        }
        std::cerr << "Session expired: " << resp.message() << std::endl;
    }

    // Fall back to login prompt - loop until successful
    while (true) {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        conn.sendLogin(username, password);
        std::string raw = conn.receive();
        osr::ServerResponse resp = proto_helpers::parseResponse(raw);

        if (!resp.success()) {
            std::cerr << "Login failed: " << resp.message() << std::endl;
            continue;
        }

        std::cout << "Logged in (user_id=" << resp.user_id() << ")" << std::endl;

        // Save token for next session if the server returned one
        if (!resp.access_token().empty()) {
            saveToken(resp.access_token());
        }
        return true;
    }
}

int main() {
    std::cout << "Press any key to begin game" << std::endl;
    std::cin.get();
    std::cout << "Connecting to server..." << std::endl;

    ConnectManager conn;
    if (!conn.isConnected()) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    if (!authenticate(conn)) {
        std::cerr << "Authentication failed." << std::endl;
        return 1;
    }

    std::cout << "==================================" << endl;
    std::cout << "||Welcome to Online: Star Rail||" << endl;
    std::cout << "==================================" << endl;
    cout << "A education purpose game made to simulate online server interaction." << endl;
    displayMenu(conn);
    return 0;
}
