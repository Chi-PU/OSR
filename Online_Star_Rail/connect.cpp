#include <iostream>
#include <cstring>
#define PORT 8080
const char* server_ip = "172.27.201.62";
#include "json.h"

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




void close_socket(socket_t sock) {
    if (sock != INVALID_SOCK) {
        closesocket(sock);
    }
#if defined(_WIN32) || defined(_WIN64)
    WSACleanup();
#endif
}


int start_socket() {
#if defined(_WIN32) || defined(_WIN64)
    // Initialize Winsock (Windows only)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }
#endif

    struct addrinfo hints, * res, * p;
    socket_t sock = INVALID_SOCK;
    

    // Setup hints for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP

    // Get address info
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", PORT);

    if (getaddrinfo(server_ip, port_str, &hints, &res) != 0) {
        std::cerr << "getaddrinfo failed" << std::endl;
#if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
#endif
        return -1;
    }

    // Try each address until we successfully connect
    for (p = res; p != NULL; p = p->ai_next) {
        // Create socket
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCK) {
            continue;
        }

        // Connect to server
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
            break;  // Success
        }

        closesocket(sock);
        sock = INVALID_SOCK;
    }

	freeaddrinfo(res); //Free the address info list

    if (sock == INVALID_SOCK) {
        std::cerr << "Connection failed" << std::endl;
#if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
#endif
        return -1;
    }
    //Build json 
	SimpleJSON json;
	json.add("action", "greet");

	//Get JSON string
	std::string json_str = json.build();
    // Send data
    send(sock, json_str.c_str(), json_str.length(), 0);
    std::cout << "Message sent" <<json_str<< std::endl;

    close_socket(sock);
    return 0;
}