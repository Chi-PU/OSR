#ifndef MENU_H
#define MENU_H

#include "command.h"
#include <vector>
#include <memory>

class Menu {
private:
    std::vector<std::unique_ptr<Command>> commands;

public:
    void addCommand(std::unique_ptr<Command> command);
    void display();
};

#endif
