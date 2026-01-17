#include "account.h"

class Account {
public:
	std::string getUsername() const {
		return username;
	}

	std::string getPassword() const {
		return password;
	}


private:
	std::string username;
	std::string password;
};
