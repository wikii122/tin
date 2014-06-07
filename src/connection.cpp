#include "connection.h"

#include <sys/types.h> 
#include <sys/socket.h>

Connection::Connection()
{
	state = ConnectionState::Intro;
}

void Connection::handle()
{
	if (incoming)
		handleIncoming();
	else
		handleOutgoing();
}

void Connection::handleIncoming()
{
}

void Connection::handleOutgoing()
{
	switch(state) {
	case ConnectionState::Intro:
		send(socket, (void*)name.c_str(), name.length(), 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)md5.c_str(), md5.length(), 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&size, 8, 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&expiry, 8, 0);
		send(socket, "\n", 1, 0);

		state = ConnectionState::PartInfo;
		break;
	case ConnectionState::PartInfo:
		state = ConnectionState::Data;
		break;
	case ConnectionState::Data:
		state = ConnectionState::PartInfo;
		break;
	}
}