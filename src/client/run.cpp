#include <iostream>
#include <unistd.h>
#include "run.h"

using namespace std;

void startServer(const char* name);
void stopServer();

const char server::server_path[] = "./server"; 

int server::start(string name)
{
	cout << "Starting server with name " << name << endl;

	startServer(name.c_str());

	return 0;
}
int server::stop()
{
	cout << "Stopping server" << endl;
	stopServer();
	return 0;
}
int server::restart(string name)
{
	cout << "Restarting server with name " << name << endl;

	stopServer();
	startServer(name.c_str());
	
	return 0;
}

void startServer(const char* name) {
	// Using fork + exec to avoid program not returning control to console.
	if(!fork()) {
		execl(server::server_path, server::server_path, name);
	}
}

void stopServer() {

}
