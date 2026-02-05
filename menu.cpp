#include "menu.h"
#include "connect_manager.h"

using namespace std;

void displayShop() {
cout << "Welcome to the shop" << endl;
    cout << "--- Oneiric Shard Refill ---" << endl;

    // Honkai: Star Rail Official Shard Tiers
    vector<ShardBundle> shopItems = {
        {"Shard Pouch", 0.99, 60, "+60 Bonus on First Purchase"},
        {"Shard Box", 4.99, 300, "+300 Bonus on First Purchase"},
        {"Shard Supply", 14.99, 980, "+980 Bonus on First Purchase"},
        {"Shard Satchel", 29.99, 1980, "+1980 Bonus on First Purchase"},
        {"Shard Case", 49.99, 3280, "+3280 Bonus on First Purchase"},
        {"Shard Contract", 99.99, 6480, "+6480 Bonus on First Purchase"}
    };

    // Display formatted table
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << endl;
    for (int i = 0; i < shopItems.size(); ++i) {
        cout << i + 1 << ". " << left << setw(15) << shopItems[i].name 
             << " | " << setw(5) << shopItems[i].shardAmount << " Shards"
             << " | $" << setw(6) << shopItems[i].price 
             << " | " << shopItems[i].bonus << endl;
    }
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << endl;

    // User Selection
    int choice;
    cout << "\nSelect a bundle to purchase (1-6) or 0 to exit: ";
    cin >> choice;

    if (choice > 0 && choice <= shopItems.size()) {
        ShardBundle selected = shopItems[choice - 1];
        cout << "\n[System] Confirming transaction for " << selected.name << "..." << endl;
        cout << "[System] " << selected.shardAmount << " Oneiric Shards have been added to your inventory." << endl;
        cout << "Total Charged: $" << selected.price << endl;
    } else {
        cout << "May this journey lead us starward! Come back soon." << endl;
    }

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
			conn.sendChoice(0, 1, 1);
			displayShop();
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
			conn.sendWishPull(0, 1);
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
