#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
class ConnectManager;

struct ShardBundle {
    string name;
    double price;
    int shardAmount;
    string bonus;
};
void displayMenu(ConnectManager& conn);
void displayShop();
void displayDaily();
void displayCharacters();

#endif // !MENU_H
