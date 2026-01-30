#include "account.h"

Account::Account(const std::string& user, const std::string& pass):
	username(user), password(pass) {
}

std::string Account::getUsername() const { return ""; }
std::string Account::getPassword() const { return ""; }