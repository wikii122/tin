/*
 * Module responsible for handling networking
 */

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

	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;

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

	void handlePacket(std::shared_ptr<GiveFileListPacket> packet);

	std::mutex queueMutex;
	std::queue<std::shared_ptr<Packet>> queue;
};