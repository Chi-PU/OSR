// socket.cpp
#include "socket.h"
#include <iostream>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
int Socket::wsa_instance_count_ = 0;
#endif

Socket::Socket() : sock_(INVALID_SOCK) {
#if defined(_WIN32) || defined(_WIN64)
    if (wsa_instance_count_ == 0) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }
    ++wsa_instance_count_;
#endif
}

Socket::~Socket() {
    close_socket();

#if defined(_WIN32) || defined(_WIN64)
    --wsa_instance_count_;
    if (wsa_instance_count_ == 0) {
        WSACleanup();
    }
#endif
}

Socket::Socket(Socket&& other) noexcept : sock_(other.sock_) {
    other.sock_ = INVALID_SOCK;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close_socket();
        sock_ = other.sock_;
        other.sock_ = INVALID_SOCK;
    }
    return *this;
}

void Socket::connect(const std::string& host, int port) {
    struct addrinfo hints, * res, * p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP

    std::string port_str = std::to_string(port);

    if (getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res) != 0) {
        throw std::runtime_error("getaddrinfo failed for " + host);
    }

    // Try each address until we successfully connect
    for (p = res; p != NULL; p = p->ai_next) {
        sock_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock_ == INVALID_SOCK) {
            continue;
        }

        // Attempt connection (blocking)
        if (::connect(sock_, p->ai_addr, p->ai_addrlen) == 0) {
            break; // Success
        }

        closesocket(sock_);
        sock_ = INVALID_SOCK;
    }

    freeaddrinfo(res);

    if (sock_ == INVALID_SOCK) {
        throw std::runtime_error("Failed to connect to " + host + ":" + port_str);
    }
}

void Socket::send(const std::string& data) {
    if (sock_ == INVALID_SOCK) {
        throw std::runtime_error("Socket not connected");
    }

    size_t total_sent = 0;
    size_t data_length = data.length();

    // Send all data (may require multiple calls)
    while (total_sent < data_length) {
        size_t bytes_sent = ::send(sock_,
            data.c_str() + total_sent,
            data_length - total_sent,
            0);

        if (bytes_sent < 0) {
#if defined(_WIN32) || defined(_WIN64)
            int error = WSAGetLastError();
            throw std::runtime_error("Send failed with error: " + std::to_string(error));
#else
            throw std::runtime_error("Send failed: " + std::string(strerror(errno)));
#endif
        }

        if (bytes_sent == 0) {
            throw std::runtime_error("Connection closed by peer during send");
        }

        total_sent += bytes_sent;
    }
}

std::string Socket::receive(size_t buffer_size) {
    if (sock_ == INVALID_SOCK) {
        throw std::runtime_error("Socket not connected");
    }

    std::string buffer(buffer_size, '\0');

    size_t bytes_received = ::recv(sock_, &buffer[0], buffer_size, 0);

    if (bytes_received < 0) {
#if defined(_WIN32) || defined(_WIN64)
        int error = WSAGetLastError();
        throw std::runtime_error("Receive failed with error: " + std::to_string(error));
#else
        throw std::runtime_error("Receive failed: " + std::string(strerror(errno)));
#endif
    }

    if (bytes_received == 0) {
        // Connection closed gracefully by peer
        return "";
    }

    buffer.resize(bytes_received);
    return buffer;
}

void Socket::close_socket() {
    if (sock_ != INVALID_SOCK) {
        closesocket(sock_);
        sock_ = INVALID_SOCK;
    }
}