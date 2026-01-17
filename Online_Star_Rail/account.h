#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <iostream>

class Account {
private:
			std::string username;
			std::string password;
public:
	Account(const std::string& user, const std::string& pass)
				: username(user), password(pass) {}
	std::string getUsername() const {
				return username;
			}
	std::string getPassword() const {
				return password;
			}
};

#endif // !ACCOUNT_H