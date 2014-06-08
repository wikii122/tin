#include "connectionhandler.h"

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include "packet/packet.h"

ConnectionHandler::ConnectionHandler()
{
	FD_ZERO(&master);
	createListenerSocket();
	FD_SET(listener, &master);
}

ConnectionHandler::~ConnectionHandler()
{
}

int ConnectionHandler::handle()
{
	while(true) {
		fd_set read;
		fd_set write;
		memcpy(&read, &master, sizeof(master));
		memcpy(&write, &master, sizeof(master));
		select(maxsock+1, &read, &write, NULL, NULL);


		if(FD_ISSET(listener, &read)) {
			sockaddr_in addr;
			unsigned int addrLen = sizeof(addr);
			int sock = accept(listener, (sockaddr*)&addr, &addrLen);

			Connection conn;
			conn.incoming = true;
			conn.socket = sock;

			if(sock > maxsock)
				maxsock = sock;

			connections.push_back(conn);
		}

		for(auto conn : connections) {
			if (conn.incoming) {
				if(FD_ISSET(conn.socket, &write) && (conn.state == ConnectionState::PartInfo))
					conn.handle();
				if(FD_ISSET(conn.socket, &read) && (conn.state == ConnectionState::Intro || conn.state == ConnectionState::Data))
					conn.handle();
			}
			else {
				if(FD_ISSET(conn.socket, &read) && (conn.state == ConnectionState::PartInfo))
					conn.handle();
				if(FD_ISSET(conn.socket, &write) && (conn.state == ConnectionState::Intro || conn.state == ConnectionState::Data))
					conn.handle();
			}
		}
	}
}
 
auto ConnectionHandler::read() -> std::string
{
	return "";
}

int ConnectionHandler::write(std::string msg)
{
	return 0;
}

void ConnectionHandler::createListenerSocket()
{
	sockaddr_in addr;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		throw std::string("ConnectionHandler::createListenerSocket: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listener, (sockaddr*)&addr, sizeof(addr)) == -1) {
		throw std::string("ConnectionHandler::createListenerSocket: Could not bind socket");
	}

	listen(listener, 5);

	maxsock = listener;
}