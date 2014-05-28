#include <iostream>
#include <unistd.h>
#include "run.h"

using namespace std;

void start(const char* name);
void stop();

int server::start(string name)
{
	cout << "Starting server with name " << name << endl;

	::start(name.c_str());

	return 0;
}
int server::stop()
{
	cout << "Stopping server" << endl;
	::stop();
	return 0;
}
int server::restart(string name)
{
	cout << "Restarting server with name " << name << endl;
	::stop();
	::start(name.c_str());
	return 0;
}

void start(const char* name) {
	// Using fork + exec to avoid program not returning control to console.
	if(!fork()) {
		execl(server::server_path, server::server_path, name);
	}
}

void stop() {

}
