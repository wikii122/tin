#include <chrono>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include "client/run.h"
#include "server.h"

using namespace std;
//! Właściwa funkcja uruchomienia serwera
void startServer(const char name[]);
//! Właściwa funkcja zatrzymania serwera.
void stopServer();

/*!
 * Uruchamia serwer z żądana nazwą.
 * \param name Żądana nazwa serwera.
 */
int server::start(string name)
{
	cout << "Starting server with name " << name << endl;

	startServer(name.c_str());

	return 0;
}

/*!
 * Zatrzymuje aktualnie działający serwer.
 */
int server::stop()
{
	if (!boost::filesystem::exists(lock_file_name)) {
		cout << "No server found, lock file does not exist" << endl;
		exit(0);
	}
	cout << "Stopping server" << endl;
	stopServer();

	return 0;
}
/*!
 * Zatrzymuje serwer, a następnie
 * uruchamia go z żadaną nazwą.
 * \param Żądana nazwa serwera.
 */
int server::restart(string name)
{
	cout << "Restarting server with name " << name << endl;

	stopServer();
	startServer(name.c_str());
	
	return 0;
}

void startServer(const char name[]) {
	// Using fork + exec to avoid program not returning control to console.
	if(!fork()) {
		std::cout << "Starting server..." << std::endl;
		execl(server::server_path, server::server_path, name);
	}
}

void stopServer() {
	int pid;
	ifstream lock_file;	
	lock_file.open(lock_file_name);
	lock_file >> pid;
	lock_file.close();

	kill(pid, SIGTERM);
	
	chrono::milliseconds duration(300);
	// Hardcoded sleep time for server to clean up.
	this_thread::sleep_for(duration);	
}
