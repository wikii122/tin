#ifndef CONNECTION_
#define CONNECTION_

#include <string>

enum ConnectionState
{
	Intro,
	PartInfo,
	Data,
};

class Connection
{
	void handleIncoming();
	void handleOutgoing();
public:
	Connection();

	void handle();

	std::string buffer;

	std::string name;
	std::string md5;
	long long size;
	long long expiry;

	long long offset;
	long long partSize;

	ConnectionState state;
	int socket;
	bool incoming;
};

#endif