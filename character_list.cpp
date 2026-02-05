#include "character_list.h"
#include "connect_manager.h"
#include <iostream>

CharacterList::CharacterList(ConnectManager& conn) : connection(conn) {}

void CharacterList::display() {
    connection.sendChoice(0, 3, 3);
    std::cout << "Here are your characters!" << std::endl;
}
