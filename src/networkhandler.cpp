#include "networkhandler.h"

#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <string.h>

NetworkHandler::NetworkHandler()
{
}

NetworkHandler::~NetworkHandler()
{
}

int NetworkHandler::handle()
{
	return 0;
}

auto NetworkHandler::read() -> std::string
{
	return "";
}

int NetworkHandler::write(std::string)
{
	return 0;
}

void NetworkHandler::createBroadcastSocket()
{
	sockaddr_in addr;
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = inet_addr("255.255.255.255");

	const bool isTrue = true;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &isTrue, sizeof(isTrue)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not setsockopt(BROADCAST)");

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not bind socket");

	//TODO: REMOVE
	if (sendto(sock, "COS", 4, 0, (sockaddr*) &addr, sizeof addr) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not sendto");
}
