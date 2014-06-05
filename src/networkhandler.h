/*
 * Module responsible for handling networking
 */

#include <string>
#include <sys/socket.h>
#include "handler.h"

class NetworkHandler: public Handler
{
	void createBroadcastSocket();
	void sendBroadcast();
public:
	NetworkHandler();
	~NetworkHandler();

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;
};