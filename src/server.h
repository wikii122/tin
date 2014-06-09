#include <string>
#include <thread>
#include <vector>
#include "clienthandler.h"
#include "handler.h"
#include "networkhandler.h"
#include "connectionhandler.h"
#include "files/storage.h"
#include "files/storage_info.h"
#ifndef SERVER_H_
#define SERVER_H_

//! Adres mutexu używanego do blokowania powstawania innych serwerów.
static const char lock_file_name[] = "server.lock";

//! Singleton będący instancją serwera.
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
	ConnectionHandler* connection_handler;

	Server();
	//! Funkcja używana do uruchomienia obsługi Handlera
	void start(Handler* handler);
public:
	//! Zwraca instancję Server
	static Server& get();
	//! Kopiowanie niedozwolone
	Server(const Server&) = delete;
	//! Destruktor dla klasy Server
	~Server();
	
	//! Setter dla nazwy serwera
	int  set_name(std::string new_name);
	//! Getter dla nazwy serwera
	auto get_name() -> std::string;
	//! Polecenie inicjujące działanie serwera.
	void serve();
	//! Getter dla ClientHandler
	ClientHandler& client();
	//! Getter dla NetworkHandler
	NetworkHandler& network();
	//! Getter dla ConnectionHandler
	ConnectionHandler& connection();
	//! Getter dla Storage
	Storage& get_storage();
	//! Getter dla Storage_info
	Storage_info& get_storage_info();
};

#endif
