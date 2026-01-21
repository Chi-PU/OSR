#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"  // localhost
#define PORT 8080
#define NUM_CLIENTS 50

void clientThread(int clientId) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    // Initialize Winsock (each thread needs to do this)
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "Client " << clientId << ": WSAStartup failed: " << result << std::endl;
        return;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "Client " << clientId << ": Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Connect to server
    std::cout << "Client " << clientId << ": Attempting to connect..." << std::endl;
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Client " << clientId << ": Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    std::cout << "Client " << clientId << ": Connected successfully!" << std::endl;

    // Send message to server
    std::string message = "Hello from client " + std::to_string(clientId);
    int sendResult = send(sock, message.c_str(), message.length(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cout << "Client " << clientId << ": Send failed: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Client " << clientId << ": Sent: " << message << std::endl;
    }

    // Receive response from server
    int recvResult = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (recvResult > 0) {
        buffer[recvResult] = '\0';
        std::cout << "Client " << clientId << ": Received: " << buffer << std::endl;
    }
    else if (recvResult == 0) {
        std::cout << "Client " << clientId << ": Connection closed by server" << std::endl;
    }
    else {
        std::cout << "Client " << clientId << ": Recv failed: " << WSAGetLastError() << std::endl;
    }

    // Close socket
    closesocket(sock);
    WSACleanup();

    std::cout << "Client " << clientId << ": Disconnected" << std::endl;
}

int main() {
    std::vector<std::thread> threads;

    std::cout << "Starting " << NUM_CLIENTS << " client connections..." << std::endl;
    std::cout << "================================================" << std::endl;

    // Create multiple client threads
    for (int i = 1; i <= NUM_CLIENTS; i++) {
        threads.push_back(std::thread(clientThread, i));

        // Small delay between connections to avoid overwhelming the server
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "================================================" << std::endl;
    std::cout << "All clients finished" << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}