#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/program_options.hpp>
#include "server.h"
#include "client/client.h"

namespace opt = boost::program_options;

using namespace std;

void signal_handler(int);
void daemonize();
void clean_up();

char lock_file_name[] = "server.lock";
int lock_file; // Descriptor for lock file.

int main(int argc, char** argv)
{
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
		}
		Server server;	
		string name = map["name"].as<string>();
		server.set_name(name);
		server.serve();
	} else {
		cout << desc;
	}
}

void signal_handler(int sig)
{
	switch(sig) {
		case SIGHUP:
			break;
		case SIGTERM:
			exit(0);
			break;
	}
}

void daemonize()
{
	// I believe we are in forked process, working in background...
	// Now things get interesting... Detaching from terminal.
	// btw, I'm doing this pure C.
	int i;
	char pid_val[10];

	setsid(); 
	// Close are opened resources... It's unspecified whether exec does that.
	for (i = getdtablesize(); i >= 0; --i) {
		close(i);
	}
	// Redirect stdin, stdout, stderr to /dev/null
	i=open("/dev/null",O_RDWR);
	dup(i); dup(i);
	
	lock_file = open(lock_file_name, O_RDWR|O_CREAT, 0640);
	
	if (lock_file < 0) exit(1);
	else if (lockf(lock_file, F_TLOCK, 0) < 0) exit(0);
	
	sprintf(pid_val, "%d\n", getpid());
	write(lock_file, pid_val, strlen(pid_val)); 

	// Now connect the signals to handler
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	// We are daemons now!!
	atexit(clean_up);
}

void clean_up()
{
	close(lock_file);
	remove(lock_file_name);	
	remove(client::SOCKET_PATH);
}
