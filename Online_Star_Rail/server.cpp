#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>

#define PORT 8080
int main(){
	//create socket
	int server_fd,new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024]= {0};
	

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0){
		std::cout << "Socket create failure" << std::endl;
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	else{
		std::cout << "Socket created success:" << server_fd << std::endl;
	}
	
	//attach socket to port
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))< 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	else {
	std::cout << "Bind success!" << std::endl;
	}

	//start listening
	if (listen(server_fd,10) < 0 ){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "Start listen success"<< std::endl;
	}
	
	//accept clientsocket
	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t *)&addrlen);
	if (new_socket < 0){
		perror("accept");
		exit(EXIT_FAILURE);
	}
	else{
		std::cout << "Accept new socket success" << std::endl;
	}

	//read data
	read(new_socket, buffer, 1024);
	std::cout << "Received: " << buffer << std::endl;
	
	//close socket
	close(new_socket);
	close(server_fd);
	return 0;
}
