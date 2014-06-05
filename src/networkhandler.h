/*
 * Module responsible for handling networking
 */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include "handler.h"

class NetworkHandler: public Handler
{
	void sendBroadcast();

	void createBroadcastSocket();

	int sock;
	sockaddr_in addr;
public:
	NetworkHandler();
	~NetworkHandler();

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;
};