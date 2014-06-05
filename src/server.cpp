#include <iostream>
#include <string>
#include <thread>
#include <boost/filesystem.hpp>
#include "server.h"

namespace filesystem=boost::filesystem;

using namespace std;

Server::Server(): 
	running(false) {
	auto rel_path = filesystem::path("file_store");
	auto abs_path = filesystem::complete(rel_path);

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
			} catch (char* err) {
				cout << err << endl;
			} catch (string err) {
				cout << err << endl;
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

void Server::register_handler(Handler* handler)
{
	handlers.push_back(handler);
}

Storage& Server::get_storage()
{
	return *storage;
}

Storage_info& Server::get_storage_info()
{
	return Storage_info::get();
}
