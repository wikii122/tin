/*
 * Module responsible for handling networking
 */

#ifndef NETWORK_HANDLER_
#define NETWORK_HANDLER_
#include "handler.h"
#include "packet/packet.h"
#include "packet/giveFileListPacket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <queue>
#include <mutex>

class NetworkHandler: public Handler
{
	void sendBroadcast();

	void createBroadcastSocket();

	int sock;
	sockaddr_in addr;

	sockaddr_in sender;
	socklen_t sendersize;

	std::vector<sockaddr_in> ownAddr;

	std::string buffer;
public:
	NetworkHandler();
	~NetworkHandler();

	void addToQueue(std::shared_ptr<Packet> msg);

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;

	int respond(std::string msg);

	void handlePacket(std::shared_ptr<GiveFileListPacket> packet);

	std::mutex queueMutex;
	std::queue<std::shared_ptr<Packet>> queue;
};
#endif
