#pragma once
#include <string>

struct LoginInfo {
	std::string username;
	std::string password;
};

class AuthHandle {
	public:
	//get login info from user
	int get_login_info();
	//unencryp login info
	int unencrypt_login_info();
	//verify login info
	int verify_login_info();
	//send back token or error message
	int send_response();
};

