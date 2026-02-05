#include "daily_rewards.h"
#include "connect_manager.h"
#include <iostream>

DailyRewards::DailyRewards(ConnectManager& conn) : connection(conn) {}

void DailyRewards::display() {
    connection.sendChoice(0, 2, 2);
    std::cout << "Here is your daily reward!" << std::endl;
}
