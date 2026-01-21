#if 0
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <vector>
#include <mutex>
#include <atomic>
#include <signal.h>
#include <chrono>

#define PORT 8080
#define MAX_THREADS 100  // Prevent resource exhaustion

// Global variables
std::atomic<int> active_threads(0);
std::atomic<bool> server_running(true);
int server_fd;

void handleClient(int client_socket) {
    active_threads++;

    char buffer[1024] = { 0 };
    std::cout << "Handling client. Active threads: " << active_threads.load() << std::endl;

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

    active_threads--;
    std::cout << "Client disconnected. Active threads: " << active_threads.load() << std::endl;
}

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal received. Shutting down..." << std::endl;
    server_running = false;
    close(server_fd);
}

void inputThread() {
    std::string input;
    while (server_running) {
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") {
            std::cout << "Shutdown command received." << std::endl;
            server_running = false;
            shutdown(server_fd, SHUT_RDWR);
            close(server_fd);
            break;
        }
        else if (input == "status") {
            std::cout << "Active threads: " << active_threads.load() << std::endl;
        }
        else if (input == "help") {
            std::cout << "Commands: quit, status, help" << std::endl;
        }
    }
}

int main() {
    struct sockaddr_in serverAddr;
    int opt = 1;

    // Register signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("server socket failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 128) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;
    std::cout << "Max concurrent threads: " << MAX_THREADS << std::endl;
    std::cout << "Type 'help' for commands" << std::endl;

    // Start input thread
    std::thread input_t(inputThread);

    // Accept loop with thread limit
    while (server_running) {
        // Check thread limit before accepting
        if (active_threads >= MAX_THREADS) {
            std::cout << "Thread limit reached (" << MAX_THREADS
                << "). Waiting..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        struct sockaddr_in cliAddr;
        socklen_t addrlen = sizeof(cliAddr);

        int new_socket = accept(server_fd, (struct sockaddr*)&cliAddr, &addrlen);

        if (new_socket < 0) {
            if (server_running) {
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
    while (active_threads > 0 && wait_count < 10) {
        std::cout << "Active threads: " << active_threads.load() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        wait_count++;
    }

    if (input_t.joinable()) {
        input_t.join();
    }

    close(server_fd);
    std::cout << "Server shutdown complete." << std::endl;

    return 0;
}
#endif