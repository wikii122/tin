#include <string>
#include <thread>
#include <vector>
#include "clienthandler.h"
#include "handler.h"
#include "networkhandler.h"
#include "files/storage.h"
#include "files/storage_info.h"
#ifndef SERVER_H_
#define SERVER_H_

static char lock_file_name[] = "server.lock";

class Server
{
	static const int no_threads = 10;

	bool running;
	std::string name;
	std::vector<Handler*> handlers;
	std::thread threads[no_threads];
	Storage* storage;
	NetworkHandler* network_handler;
	ClientHandler* client_handler;

	Server();
	void start(Handler* handler);
public:
	static Server& get();
	Server(const Server&) = delete;
	~Server();
	
	int  set_name(std::string new_name);
	auto get_name() -> std::string;
	void serve();
	ClientHandler& client();
	NetworkHandler& network();
	Storage& get_storage();
	Storage_info& get_storage_info();
};

#endif
