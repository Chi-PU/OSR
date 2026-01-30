#include "connect_manager.h"
#include "game.pb.h" // now generates osr:: namespace
#include <cstring>
#include <stdexcept>

namespace proto_helpers {
    std::string buildLoginRequest(const std::string& username, const std::string& password);
    std::string buildGachaPullRequest(int user_id, int pull_count);
    std::string buildChoiceRequest(int user_id, int choice, int scenario_id);
}

ConnectManager::ConnectManager(const std::string& server_ip, int port) {
    try {
        sock_.connect(server_ip, port);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
    }
}

void ConnectManager::sendRaw(const std::string& data) {
    // 4-byte length prefix (network byte order) followed by payload
    uint32_t len = htonl(static_cast<uint32_t>(data.size()));
    std::string frame(reinterpret_cast<const char*>(&len), 4);
    frame += data;
    sock_.send(frame);
}

std::string ConnectManager::receive() {
    // Read the 4-byte length prefix
    std::string header = sock_.receive(4);
    if (header.size() < 4) {
        throw std::runtime_error("Connection closed or incomplete header");
    }

    uint32_t net_len;
    std::memcpy(&net_len, header.data(), 4);
    uint32_t payload_len = ntohl(net_len);

    if (payload_len == 0) {
        return "";
    }

    // Read the full payload
    std::string payload;
    payload.reserve(payload_len);
    while (payload.size() < payload_len) {
        std::string chunk = sock_.receive(payload_len - payload.size());
        if (chunk.empty()) {
            throw std::runtime_error("Connection closed while reading payload");
        }
        payload += chunk;
    }

    return payload;
}

void ConnectManager::sendLogin(const std::string& username, const std::string& password) {
    std::string data = proto_helpers::buildLoginRequest(username, password);
    sendRaw(data);
}

void ConnectManager::sendGachaPull(int user_id, int pull_count) {
    std::string data = proto_helpers::buildGachaPullRequest(user_id, pull_count);
    sendRaw(data);
}

void ConnectManager::sendChoice(int user_id, int choice, int scenario_id) {
    std::string data = proto_helpers::buildChoiceRequest(user_id, choice, scenario_id);
    sendRaw(data);
}
