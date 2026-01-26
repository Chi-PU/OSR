//first code
#include <iostream>

#include "menu.h"

using namespace std;



	int main() {
		std::cout << "Press any key to begin game" << std::endl;
		std::cin.get();
		std::cout << "Connecting to server..." << std::endl;
		/*int status = start_socket();
		if (status == -1) {
			std::cerr << "Failed to connect to server. Exiting game." << std::endl;
			return -1;
		}*/
		std::cout << "==================================" << endl;
		std::cout << "||Welcome to Online: Star Rail||" << endl;
		std::cout << "=================================="<< endl;
		cout << "A education purpose game made to simulate online server interaction." << endl;
		displayMenu();
		return 0;

	}

