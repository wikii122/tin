/*
 * Module responsible for reading input from console client.
 * Opens it's own socket, with hardcoded file address, and
 * keeps it open for life of the process.
 */

#include <string>
#include <sys/socket.h>
#include "handler.h"

class ClientHandler: public Handler
{
	class Connection;
	Connection* connection = nullptr;
	Connection* accept();
public:
	ClientHandler();
	~ClientHandler();

	virtual int handle() override; //Blocking!
	virtual auto read() -> std::string override; // Blocking
	virtual int write(std::string) override;
};

class ClientHandler::Connection
{
	int socket_fd;
public:
	Connection(int fd);
	~Connection();
	auto read() -> std::string;
	int write(const char msg[], int len);
};
