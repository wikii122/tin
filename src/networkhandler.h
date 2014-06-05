/*
 * Module responsible for handling networking
 */

#include "handler.h"

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
public:
	NetworkHandler();
	~NetworkHandler();

	void addToQueue(std::string msg);

	virtual int handle() override; //Blocking!

	std::mutex queueMutex;
	std::queue<std::string> queue;
};