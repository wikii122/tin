#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "clienthandler.h"
#include "client/client.h"

using namespace std;

ClientHandler::ClientHandler(): 
	Handler()
{
	struct sockaddr_un address;

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);		

	// Zero address struct, fill it with address.
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;	
	strcpy(address.sun_path, client::SOCKET_PATH);
	bind(socket_fd, (struct sockaddr*) &address, sizeof(address));
	listen(socket_fd, 5);
}

ClientHandler::~ClientHandler()
{
	close(socket_fd);
}

auto ClientHandler::accept() -> Connection* 
{
	int result;

	result = ::accept(socket_fd, 0, 0);
	if (connection != nullptr)
		delete connection;
	connection = new Connection(result);
	return connection;
}

int ClientHandler::write(char msg[], int len)
{
	return connection->write(msg, len);
}

string ClientHandler::read()
{
	return connection->read();
}

int ClientHandler::handle()
{
	accept();
	// Now new connection needs to be handled.
	string msg = read();
	// TODO input dispatch...
	// TODO response handling...
	write("SAMPLE", 7);

	return 0;
}

ClientHandler::Connection::Connection(int fd): 
	socket_fd(fd) 
{

}

ClientHandler::Connection::~Connection()
{
	close(socket_fd);
}

int ClientHandler::Connection::write(char msg[], int len)
{
	return ::write(socket_fd, msg, len);
}

string ClientHandler::Connection::read()
{
	// This requires thing to parse JSON input.
	char buffer[11];
	::read(socket_fd, buffer, 10);
	return string(buffer, buffer+10);
}
