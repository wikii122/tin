/*
 * Module responsible for handling TCP networking
 */

#ifndef CONNECTION_HANDLER_
#define CONNECTION_HANDLER_
#include "handler.h"
#include "packet/packet.h"
#include "packet/giveFileListPacket.h"
#include "packet/iHavePacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "connection.h"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <queue>
#include <mutex>

class ConnectionHandler: public Handler
{
	int listener;

	std::vector<Connection> connections;

	fd_set master;
	int maxsock;

	void createListenerSocket();
public:
	ConnectionHandler();
	~ConnectionHandler();

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;
};
#endif
