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
	sockaddr_in addr;
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		throw std::string("NetworkHandler::read: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = inet_addr("192.168.1.255");

	const int isTrue = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &isTrue, sizeof(isTrue)) == -1)
		throw std::string("NetworkHandler::read: Could not setsockopt(BROADCAST)");

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::string("NetworkHandler::read: Could not bind socket");

	char msg[1024];

	sockaddr_in sender;
	socklen_t sendersize = sizeof sender;
	memset(&sender, 0, sizeof sender);

	if (recvfrom(sock, msg, 1024, 0, (sockaddr*) &sender, &sendersize) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::read: Could not recvfrom");
	}

	printf(msg);
	fflush(stdout);
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
	addr.sin_addr.s_addr = inet_addr("192.168.1.255");

	const int isTrue = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &isTrue, sizeof(isTrue)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not setsockopt(BROADCAST)");

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not bind socket");

	//TODO: REMOVE
	if (sendto(sock, "COS", 4, 0, (sockaddr*) &addr, sizeof addr) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::createBroadcastSocket: Could not sendto");
	}
}
