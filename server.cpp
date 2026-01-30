#if 0
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <vector>
#include <mutex>
#include <signal.h>
#include <chrono>
#include "global.h"

#define PORT 8080
#define MAX_THREADS 100  // Prevent resource exhaustion

using namespace Global;

void handleClient(int client_socket) {
    Global::activethreads++;

    char buffer[1024] = { 0 };
    std::cout << "Handling client. Active threads: " << Global::activethreads.load() << std::endl;

    // Read data
    int valread = read(client_socket, buffer, 1024);
    if (valread > 0) {
        std::cout << "Received: " << buffer << std::endl;
    }

    // Send response
    const char* response = "Hello from server";
    send(client_socket, response, strlen(response), 0);

    // Close client socket
    close(client_socket);

    Global::activethreads--;
    std::cout << "Client disconnected. Active threads: " << Global::activethreads.load() << std::endl;
}

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal received. Shutting down..." << std::endl;
    Global::server_running = false;
    close(Global::server_fd);
}

void inputThread() {
    std::string input;
    while (Global::server_running) {
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") {
            std::cout << "Shutdown command received." << std::endl;
            Global::server_running = false;
            shutdown(Global::server_fd, SHUT_RDWR);
            close(Global::server_fd);
            break;
        }
        else if (input == "status") {
            std::cout << "Active threads: " << Global::activethreads.load() << std::endl;
        }
        else if (input == "help") {
            std::cout << "Commands: quit, status, help" << std::endl;
        }
    }
}

int login() {
    std::cout << "User logged in successful" << std::endl;
    return 0;
}

int main() {
    struct addrinfo hints, * res, * p;
    int opt = 1;

    // Register signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Setup hints for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // For binding to all interfaces

    // Get address info
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", PORT);

    if (getaddrinfo(NULL, port_str, &hints, &res) != 0) {
        std::cerr << "getaddrinfo failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Try each address until we successfully bind
    for (p = res; p != NULL; p = p->ai_next) {
        // Create server socket
        Global::server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (Global::server_fd < 0) {
            continue;
        }

        // Set socket options
        if (setsockopt(Global::server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            close(Global::server_fd);
            continue;
        }

        // Bind
        if (bind(Global::server_fd, p->ai_addr, p->ai_addrlen) == 0) {
            break;  // Success
        }

        close(Global::server_fd);
    }

    freeaddrinfo(res);

    if (p == NULL) {
        std::cerr << "Failed to bind to any address" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(Global::server_fd, 128) < 0) {
        perror("listen");
        close(Global::server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;
    std::cout << "Max concurrent threads: " << MAX_THREADS << std::endl;
    std::cout << "Type 'help' for commands" << std::endl;

    // Start input thread
    std::thread input_t(inputThread);

    // Accept loop with thread limit
    while (Global::server_running) {
        // Check thread limit before accepting
        if (Global::activethreads >= MAX_THREADS) {
            std::cout << "Thread limit reached (" << MAX_THREADS
                << "). Waiting..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        struct sockaddr_storage cliAddr;  // Changed to sockaddr_storage for IPv4/IPv6
        socklen_t addrlen = sizeof(cliAddr);

        int new_socket = accept(Global::server_fd, (struct sockaddr*)&cliAddr, &addrlen);

        if (new_socket < 0) {
            if (Global::server_running) {
                perror("accept failed");
            }
            break;
        }

        std::cout << "New connection accepted" << std::endl;

        // Create detached thread
        std::thread(handleClient, new_socket).detach();
    }

    // Cleanup
    std::cout << "Shutting down. Waiting for active threads..." << std::endl;

    int wait_count = 0;
    while (Global::activethreads > 0 && wait_count < 10) {
        std::cout << "Active threads: " << Global::activethreads.load() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        wait_count++;
    }

    if (input_t.joinable()) {
        input_t.join();
    }

    close(Global::server_fd);
    std::cout << "Server shutdown complete." << std::endl;

    return 0;
}
#endif