#pragma once

#include "socket.h"
#include "ssl.h"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <string>

#define PORT 9000

class ConnectManager {
public:
	static constexpr const char* DEFAULT_SERVER_IP = "127.0.0.1";

	// ca_cert_path: path to server CA cert for verification.
	// If empty (default), accepts self-signed server certificates.
	ConnectManager(const std::string& server_ip = DEFAULT_SERVER_IP, int port = PORT,
	               const std::string& ca_cert_path = "");
	~ConnectManager() = default;

	ConnectManager(const ConnectManager&) = delete;
	ConnectManager& operator=(const ConnectManager&) = delete;
	ConnectManager(ConnectManager&&) = default;
	ConnectManager& operator=(ConnectManager&&) = default;

	bool isConnected() const { return sock_.is_connected() && sock_.is_tls(); }

	void sendRaw(const std::string& data);
	std::string receive();

	void sendLogin(const std::string& username, const std::string& password);
	void sendSignup(const std::string& username, const std::string& password);
	void sendWishPull(int user_id, int pull_count);
	void sendChoice(int user_id, int choice, int scenario_id);
	void sendTokenAuth(const std::string& jwt_token);
	void sendShopRequest(int user_id, int item_id, int quantity);
	void sendPurchase(int user_id, int bundle_id, double price, int shard_amount);

private:
	SSLContext ssl_ctx_;
	Socket sock_;
};
