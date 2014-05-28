#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "command.h"
#include "client.h"
	
using namespace std;

/*
 * This function calls server and ignores input,
 * unless an error happens
 */
int communicate::call(string msg)
{
	callForResponse(msg);
	return 0;
}

/*
 * This function calls server and returns response.
 */
string communicate::callForResponse(string msg)
{	
	int socket_fd, state, len, result;
	struct sockaddr_un address;
	
	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		cout << "Could not create socket" << endl;;
		throw "Socket";
	}
	
	// Zero address structure
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, client::SOCKET_PATH);
	state = connect(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un));
	
	if(state == -1) {
		cout << "Socket file does not exist...\nIs server running?" << endl;
		exit(1);	
	}
	
	len = msg.length();

	write(socket_fd, msg.c_str(), len);
	// TODO add timeout
	// TODO use something more meaningful than int.
	// TODO merge call for response here.
	read(socket_fd, &result, 4);

	return "";
}
