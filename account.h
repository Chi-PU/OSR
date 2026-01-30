#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <iostream>

class Account {
private:
			std::string username;
			std::string password;
public:
	Account(const std::string& user, const std::string& pass);
	std::string getUsername() const;
	std::string getPassword() const;
			
};

#endif // !ACCOUNT_H