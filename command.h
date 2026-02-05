#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <functional>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};

class CallbackCommand : public Command {
private:
    std::string name_;
    std::function<void()> callback_;

public:
    CallbackCommand(std::string name, std::function<void()> callback);
    void execute() override;
    std::string getName() const override;
};

class ExitCommand : public Command {
private:
    bool& running_;

public:
    explicit ExitCommand(bool& runningFlag);
    void execute() override;
    std::string getName() const override;
};

#endif
