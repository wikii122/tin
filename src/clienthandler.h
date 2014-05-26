/*
 * Module responsible for reading input from console client.
 * Opens it's own socket, with hardcoded file address, and
 * keeps it open for life of the process.
 */
#include <sys/socket.h>

class ClientHandler
{
public:
	ClientHandler();
	~ClientHandler();
	// Check for input from user.
	int handleRequests();
};
