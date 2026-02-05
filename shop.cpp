#include "shop.h"
#include "connect_manager.h"
#include <iostream>
#include <iomanip>

Shop::Shop(ConnectManager& conn) : connection(conn) {
    shopItems = {
        {"Shard Pouch", 0.99, 60, "+60 Bonus on First Purchase"},
        {"Shard Box", 4.99, 300, "+300 Bonus on First Purchase"},
        {"Shard Supply", 14.99, 980, "+980 Bonus on First Purchase"},
        {"Shard Satchel", 29.99, 1980, "+1980 Bonus on First Purchase"},
        {"Shard Case", 49.99, 3280, "+3280 Bonus on First Purchase"},
        {"Shard Contract", 99.99, 6480, "+6480 Bonus on First Purchase"}
    };
}

void Shop::displayItems() {
    std::cout << "--- Oneiric Shard Refill ---" << std::endl;
    std::cout << std::setfill('-') << std::setw(65) << "-" << std::setfill(' ') << std::endl;

    for (size_t i = 0; i < shopItems.size(); ++i) {
        std::cout << i + 1 << ". " << std::left << std::setw(15) << shopItems[i].name
                  << " | " << std::setw(5) << shopItems[i].shardAmount << " Shards"
                  << " | $" << std::setw(6) << shopItems[i].price
                  << " | " << shopItems[i].bonus << std::endl;
    }

    std::cout << std::setfill('-') << std::setw(65) << "-" << std::setfill(' ') << std::endl;
}

void Shop::processPurchase(int choice) {
    if (choice > 0 && choice <= static_cast<int>(shopItems.size())) {
        ShardBundle selected = shopItems[choice - 1];
        connection.sendPurchase(0, choice, selected.price, selected.shardAmount);
        std::cout << "\n[System] Confirming transaction for " << selected.name << "..." << std::endl;
        std::cout << "[System] " << selected.shardAmount << " Oneiric Shards have been added to your inventory." << std::endl;
        std::cout << "Total Charged: $" << selected.price << std::endl;
    } else {
        std::cout << "May this journey lead us starward! Come back soon." << std::endl;
    }
}

void Shop::display() {
    connection.sendChoice(0, 1, 1);
    std::cout << "Welcome to the shop" << std::endl;
    displayItems();

    int choice;
    std::cout << "\nSelect a bundle to purchase (1-6) or 0 to exit: ";
    std::cin >> choice;

    processPurchase(choice);
}
