#include "menu.h"
#include "connect_manager.h"

using namespace std;

void displayShop() {
	cout << "Welcome to the Shop!" << endl;
}

void displayDaily() {
	cout << "Here is your daily reward!" << endl;
}

void displayCharacters() {
	cout << "Here are your characters!" << endl;
}

void displayMenu(ConnectManager& conn) {
	int x;
	bool running = true;

	while (running) {
		cout << "1. Shop" << endl;
		cout << "2. Daily" << endl;
		cout << "3. Characters" << endl;
		cout << "4. Wish" << endl;
		cout << "5. Bag" << endl;
		cout << "6. Party" << endl;
		cout << "7. Exit" << endl;
		cin >> x;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid input, please enter a number." << endl;
			continue;
		}

		switch (x) {
		case 1:
			displayShop();
			conn.sendChoice(0, 1, 1);
			break;
		case 2:
			displayDaily();
			conn.sendChoice(0, 2, 2);
			break;
		case 3:
			displayCharacters();
			conn.sendChoice(0, 3, 3);
			break;
		case 4:
			cout << "Making a wish..." << endl;
			conn.sendGachaPull(0, 1);
			break;
		case 5:
			cout << "Bag feature coming soon!" << endl;
			conn.sendChoice(0, 5, 5);
			break;
		case 6:
			cout << "Party feature coming soon!" << endl;
			conn.sendChoice(0, 6, 6);
			break;
		case 7:
			running = false;
			break;
		default:
			cout << "Invalid option, please try again." << endl;
			break;
		}
	}
}
