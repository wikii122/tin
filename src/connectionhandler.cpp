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

ConnectionHandler* ConnectionHandler::getInstance()
{
	static ConnectionHandler* instance = new ConnectionHandler();
	return instance;
}

ConnectionHandler::ConnectionHandler()
{
}

ConnectionHandler::~ConnectionHandler()
{
}

int ConnectionHandler::handle()
{
}

auto ConnectionHandler::read() -> std::string
{
}

int ConnectionHandler::write(std::string msg)
{
}