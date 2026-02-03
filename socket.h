#pragma once
#include <string>
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define INVALID_SOCK INVALID_SOCKET
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
typedef int socket_t;
#define INVALID_SOCK -1
#define closesocket close
#endif

#include <openssl/ssl.h>

class Socket {
public:
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    // Simple blocking operations
    void connect(const std::string& host, int port);
    void send(const std::string& data);
    std::string receive(size_t buffer_size = 4096);

    // Upgrade connection to TLS using the given SSL_CTX
    void upgrade_to_tls(SSL_CTX* ctx);

    bool is_connected() const { return sock_ != INVALID_SOCK; }
    bool is_tls() const { return ssl_ != nullptr; }

private:
    socket_t sock_;
    SSL* ssl_;

#if defined(_WIN32) || defined(_WIN64)
    static int wsa_instance_count_;
#endif

    void close_socket();
};
