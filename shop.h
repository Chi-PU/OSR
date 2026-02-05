#ifndef SHOP_H
#define SHOP_H

#include <vector>
#include <string>

class ConnectManager;

struct ShardBundle {
    std::string name;
    double price;
    int shardAmount;
    std::string bonus;
};

class Shop {
private:
    ConnectManager& connection;
    std::vector<ShardBundle> shopItems;

    void displayItems();
    void processPurchase(int choice);

public:
    explicit Shop(ConnectManager& conn);
    void display();
};

#endif
