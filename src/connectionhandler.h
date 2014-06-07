/*
 * Module responsible for handling networking
 */

#ifndef CONNECTION_HANDLER_
#define CONNECTION_HANDLER_
#include "handler.h"
#include "packet/packet.h"
#include "packet/giveFileListPacket.h"
#include "packet/iHavePacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "packet/objectionPacket.h"
#include "packet/forgetPacket.h"
#include "packet/iForgotPacket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <queue>
#include <mutex>

class ConnectionHandler: public Handler
{
	ConnectionHandler();
	~ConnectionHandler();
public:
	static ConnectionHandler* getInstance();

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;
};
#endif
