#include "connect_manager.h"

ConnectManager::ConnectManager()
	{
	 try {
		 Socket sock;
		 sock.connect(server_ip, PORT);
	 }
	 catch (const std::runtime_error& e) {
		 perror(e.what());
	 }
	}

void doGameLoop() {
	Socket sock;
}