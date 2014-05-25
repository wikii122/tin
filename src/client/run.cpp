#include <iostream>
#include "run.h"

using namespace std;
int server::start(string name)
{
	cout << "Starting server with name " << name << endl;
	return 0;
}
int server::stop()
{
	cout << "Stopping server" << endl;
	return 0;
}
int server::restart(string name)
{
	cout << "Restarting server with name " << name << endl;
	return 0;
}

