#ifndef CHARACTER_LIST_H
#define CHARACTER_LIST_H

class ConnectManager;

class CharacterList {
private:
    ConnectManager& connection;

public:
    explicit CharacterList(ConnectManager& conn);
    void display();
};

#endif
