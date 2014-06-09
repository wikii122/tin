#ifndef CONNECTION_
#define CONNECTION_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

enum ConnectionState
{
	Intro,
	PartInfo,
	Data,
	Finished,
};

class Connection
{
	void handleIncoming();
	void handleOutgoing();
public:
	Connection();

	sockaddr_in addr;

	void handle();

	char* buffer;

	char* fileData;
	int transferred;

	std::string name;
	std::string md5;
	long long size;
	long long expiry;

	long long offset;
	long long partSize;

	bool original;

	ConnectionState state;
	int socket;
	bool incoming;
};

#endif