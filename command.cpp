#include "command.h"
#include <iostream>
#include <utility>

CallbackCommand::CallbackCommand(std::string name, std::function<void()> callback)
    : name_(std::move(name)), callback_(std::move(callback)) {}

void CallbackCommand::execute() {
    if (callback_) {
        callback_();
    }
}

std::string CallbackCommand::getName() const {
    return name_;
}

ExitCommand::ExitCommand(bool& runningFlag)
    : running_(runningFlag) {}

void ExitCommand::execute() {
    running_ = false;
    std::cout << "Exiting... Goodbye!" << std::endl;
}

std::string ExitCommand::getName() const {
    return "Exit";
}
