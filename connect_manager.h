#pragma once

#include "socket.h"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <string>

#define PORT 9000

class ConnectManager {
public:
	static constexpr const char* DEFAULT_SERVER_IP = "127.0.0.1";

	ConnectManager(const std::string& server_ip = DEFAULT_SERVER_IP, int port = PORT);
	~ConnectManager() = default;

	ConnectManager(const ConnectManager&) = delete;
	ConnectManager& operator=(const ConnectManager&) = delete;
	ConnectManager(ConnectManager&&) = default;
	ConnectManager& operator=(ConnectManager&&) = default;

	bool isConnected() const { return sock_.is_connected(); }

	void sendRaw(const std::string& data);
	std::string receive();

	void sendLogin(const std::string& username, const std::string& password);
	void sendGachaPull(int user_id, int pull_count);
	void sendChoice(int user_id, int choice, int scenario_id);

private:
	Socket sock_;
};
