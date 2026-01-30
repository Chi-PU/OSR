//first code
#include <iostream>

#include "menu.h"
#include "connect_manager.h"

using namespace std;

	int main() {
		std::cout << "Press any key to begin game" << std::endl;
		std::cin.get();
		std::cout << "Connecting to server..." << std::endl;

		ConnectManager conn;
		if (!conn.isConnected()) {
			std::cerr << "Failed to connect to server. Try again." << std::endl;
			main();
		}
		conn.sendLogin("test_user", "password123");
		std::cout << "==================================" << endl;
		std::cout << "||Welcome to Online: Star Rail||" << endl;
		std::cout << "=================================="<< endl;
		cout << "A education purpose game made to simulate online server interaction." << endl;
		displayMenu(conn);
		return 0;

	}
