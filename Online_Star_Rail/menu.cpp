#include "menu.h"


void displayShop() {
	std::cout << "Welcome to the Shop!" << endl;
}

void displayDaily() {
	cout << "Here is your daily reward!" << endl;
}

void displayCharacters() {
	cout << "Here are your characters!" << endl;
}

void displayMenu() {
	int x;
	cout << "1. Enter Shop" << endl;
	cout << "2. Open Daily" << endl;
	cout << "3. Open Characters" << endl;
	cout << "4. Exit Game" << endl;
	cin >> x;
	//loging can be added here to track user choices
	switch (x) {
	case 1:
		displayShop();
		break;
	case 2:
		displayDaily();
		break;
	case 3:
		displayCharacters();
		break;
	case 4:
		exit(0);
		break;
	default:
		cout << "Invalid option, please try again." << endl;
		displayMenu();
		break;

	}
}