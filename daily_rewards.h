#ifndef DAILY_REWARDS_H
#define DAILY_REWARDS_H

class ConnectManager;

class DailyRewards {
private:
    ConnectManager& connection;

public:
    explicit DailyRewards(ConnectManager& conn);
    void display();
};

#endif
