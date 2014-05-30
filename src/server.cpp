#include <string>
#include <thread>
#include "server.h"

using namespace std;

Server::Server(): 
running(false)
{
	
}

Server::~Server()
{
	running = false;
	for (Handler* handler: handlers) {
		delete handler;
	}
}

int Server::set_name(string new_name)
{
	name = new_name;	
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
	for (Handler* handler: handlers) {
		auto fun = [this](Handler* handler) {
			while (this->running) handler->handle();
		};
		threads[i++] = thread(fun, handler);
		if (i > no_threads) {
			throw string("Too many jobs passed to server");
		}
	}
}

void Server::register_handler(Handler* handler)
{
	handlers.push_back(handler);
}
