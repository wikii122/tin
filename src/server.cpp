#include <iostream>
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
	running = true;
	for (Handler* handler: handlers) {
		auto fun = [this](Handler* handler) {
			while (this->running) try {
				handler->handle();
			} catch (char* err) {
				cout << err << endl;
			} catch (string err) {
				cout << err << endl;
			} catch (exception err) {
				cout << err.what() << endl;
			} catch (...) {
				// pass
			}
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
