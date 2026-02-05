#include "menu.h"
#include <iostream>

void Menu::addCommand(std::unique_ptr<Command> command) {
    commands.push_back(std::move(command));
}

void Menu::display() {
    std::cout << "\n=== Main Menu ===" << std::endl;

    for (size_t i = 0; i < commands.size(); ++i) {
        std::cout << i + 1 << ". " << commands[i]->getName() << std::endl;
    }

    int choice;
    std::cout << "Select an option: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input, please enter a number." << std::endl;
        return;
    }

    if (choice > 0 && choice <= static_cast<int>(commands.size())) {
        commands[choice - 1]->execute();
    } else {
        std::cout << "Invalid option, please try again." << std::endl;
    }
}
