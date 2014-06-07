#include <iostream>
#include <string>
#include <thread>
#include <boost/filesystem.hpp>
#include "clienthandler.h"
#include "networkhandler.h"
#include "server.h"

namespace filesystem=boost::filesystem;

using namespace std;

Server::Server(): 
	running(false) {
	auto rel_path = filesystem::path("file_store");
	auto abs_path = filesystem::complete(rel_path);

	client_handler = new ClientHandler();
	network_handler = NetworkHandler::getInstance();

	handlers.push_back(client_handler);
	handlers.push_back(network_handler);

	if (!filesystem::exists(abs_path)) {
		filesystem::create_directories(abs_path);
	} 

	storage = new Storage(abs_path.string<string>());
}

Server& Server::get()
{
	static Server instance;
	return instance;
}

Server::~Server()
{
	running = false;
	delete storage;
	delete client_handler;
}

int Server::set_name(string new_name)
{
	name = new_name;	
	Storage_info::get().set_name(new_name);
	// TODO register in network if appropiate handler...
	// TODO check if name is valid
	return 0;
}

string Server::get_name()
{
	return name;
}

void Server::serve()
{
	int i = 0;
	running = true;
	for (Handler* handler: handlers) {
		auto fun = [this](Handler* handler) {
			while (this->running) try {
				handler->handle();
			} catch (exception err) {
				cout << err.what() << endl;
				terminate();
			} catch (char* err) {
				cout << err << endl;
				terminate();
			} catch (const char* err) {
				cout << err << endl;
				terminate();
			} catch (string err) {
				cout << err << endl;
				terminate();
			} /*catch (...) {
				// pass
			}*/
		};
		threads[i] = thread(fun, handler);
		if (i == no_threads) {
			throw string("Too many jobs passed to server");
		} else if (i > 0) {
			threads[i].detach();
		}
		i++;
	}
	threads[0].join();
}

Storage& Server::get_storage()
{
	return *storage;
}

Storage_info& Server::get_storage_info()
{
	return Storage_info::get();
}

ClientHandler& Server::client()
{
	return *client_handler;
}

NetworkHandler& Server::network()
{
	return *network_handler;
}
