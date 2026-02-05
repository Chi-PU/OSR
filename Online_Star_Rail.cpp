#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "menu.h"
#include "shop.h"
#include "daily_rewards.h"
#include "character_list.h"
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

    // Show login/signup menu
    while (true) {
        std::cout << "\n1. Login" << std::endl;
        std::cout << "2. Sign Up" << std::endl;
        std::cout << "Choose an option: ";

        std::string choice_str;
        std::getline(std::cin, choice_str);
        int choice = 0;
        try {
            choice = std::stoi(choice_str);
        } catch (...) {
            std::cerr << "Invalid option. Please enter 1 or 2." << std::endl;
            continue;
        }

        if (choice == 1) {
            // Login
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
                    std::cout << "Try again? (y/n): ";
                    std::string retry;
                    std::getline(std::cin, retry);
                    if (retry != "y" && retry != "Y") {
                        break;  // Go back to login/signup menu
                    }
                    continue;
                }

                std::cout << "Logged in (user_id=" << resp.user_id() << ")" << std::endl;

                if (!resp.access_token().empty()) {
                    saveToken(resp.access_token());
                }
                return true;
            }
        } else if (choice == 2) {
            // Sign Up
            while (true) {
                std::string username, password, confirm_password;
                std::cout << "Choose a username: ";
                std::getline(std::cin, username);
                std::cout << "Choose a password: ";
                std::getline(std::cin, password);
                std::cout << "Confirm password: ";
                std::getline(std::cin, confirm_password);

                if (password != confirm_password) {
                    std::cerr << "Passwords do not match. Please try again." << std::endl;
                    continue;
                }

                conn.sendSignup(username, password);
                std::string raw = conn.receive();
                osr::ServerResponse resp = proto_helpers::parseResponse(raw);

                if (!resp.success()) {
                    std::cerr << "Signup failed: " << resp.message() << std::endl;
                    std::cout << "Try again? (y/n): ";
                    std::string retry;
                    std::getline(std::cin, retry);
                    if (retry != "y" && retry != "Y") {
                        break;  // Go back to login/signup menu
                    }
                    continue;
                }

                std::cout << "Account created successfully!" << std::endl;
                std::cout << "Logged in (user_id=" << resp.user_id() << ")" << std::endl;

                if (!resp.access_token().empty()) {
                    saveToken(resp.access_token());
                }
                return true;
            }
        } else {
            std::cerr << "Invalid option. Please enter 1 or 2." << std::endl;
        }
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

    Shop shop(conn);
    DailyRewards daily(conn);
    CharacterList characters(conn);

    Menu menu;
    bool running = true;

    menu.addCommand(make_unique<CallbackCommand>("Shop",
        [&shop]() { shop.display(); }));

    menu.addCommand(make_unique<CallbackCommand>("Daily",
        [&daily]() { daily.display(); }));

    menu.addCommand(make_unique<CallbackCommand>("Characters",
        [&characters]() { characters.display(); }));

    menu.addCommand(make_unique<CallbackCommand>("Wish",
        [&conn]() {
            cout << "Making a wish..." << endl;
            conn.sendWishPull(0, 1);
        }));

    menu.addCommand(make_unique<CallbackCommand>("Bag",
        [&conn]() {
            cout << "Bag feature coming soon!" << endl;
            conn.sendChoice(0, 5, 5);
        }));

    menu.addCommand(make_unique<CallbackCommand>("Party",
        [&conn]() {
            cout << "Party feature coming soon!" << endl;
            conn.sendChoice(0, 6, 6);
        }));

    menu.addCommand(make_unique<ExitCommand>(running));

    while (running) {
        menu.display();
    }

    return 0;
}
