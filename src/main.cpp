#include <fcntl.h>
#include <fstream>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "clienthandler.h"
#include "server.h"
#include "client/client.h"

#include "packet/helloPacket.h"

namespace opt = boost::program_options;

using namespace std;

void initialize_server(string name);
void signal_handler(int);
void daemonize();
void clean_up();

int main(int argc, char** argv)
try {
	//ConnectionHandler h;
	//while(true) h.handle();
	
	if (argc > 3) {
		argc = 2;
	}
	opt::options_description desc("Server options");
	desc.add_options()
		("help", "Print this message")
		("name", "Server name")
		("foreground", "Run in foreground");

	opt::positional_options_description pos;
	pos.add("name", 1);
	opt::variables_map map;
	opt::store(opt::command_line_parser(argc, argv).options(desc).positional(pos).run(), map);
	if (map.count("name")) {
		if (!map.count("foreground")) {
			daemonize();
		} else {
			signal(SIGINT, signal_handler);
			atexit(clean_up);
		}
		string name = map["name"].as<string>();
		initialize_server(name);
	} else {
		cout << desc;
	}
} catch (char err[]) {
	cerr << err << endl;
} catch (string err) {
	cerr << err << endl;
} /*catch (exception err) {
	cerr << err.what() << endl;
} catch (...) {
	cerr << "Unknown error, terminating. We are sorry." << endl;
}*/

void signal_handler(int sig)
{
	switch(sig) {
		case SIGHUP:
			break;
		case SIGTERM:
		case SIGINT:
			clean_up();
			exit(0);
			break;
	}
}

void initialize_server(string name)
{
	Server& server = Server::get();	
	server.set_name(name);
	server.serve();
}

void daemonize()
{
	// I believe we are in forked process, working in background...
	// Now things get interesting... Detaching from terminal.
	int i;
	int pid_val = getpid();

	if (boost::filesystem::exists(lock_file_name )) {
		cout << "Server already running or wasn't properly closed." << endl;
		cout << "If server is not running, please remove lock file manually" << endl;
		exit(0);
	} else {
		ofstream lock_file;
		lock_file.open(lock_file_name, ios::out);
		lock_file << pid_val << flush;
		lock_file.close();
	}

	setsid(); 

	// Close are opened resources... It's unspecified whether exec does that.
	for (i = getdtablesize(); i >= 0; --i) {
		close(i);
	}
	// Redirect stdin, stdout, stderr to /dev/null
	i = open("/dev/null", O_RDWR);
	dup(i); dup(i);
	
	// Now connect the signals to handler
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	// We are daemons now!!
	atexit(clean_up);
}

void clean_up()
{
	remove(lock_file_name);	
	remove(client::SOCKET_PATH);
}
