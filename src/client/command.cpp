#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "command.h"
	
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
	int socket, state, len, result;
	struct sockaddr_un address;
	
	socket = (PF_UNIX, SOCK_STREAM);
	if (socket == -1) {
		cout << "Could not create socket" << endl;;
		throw "Socket";
	}
	
	// Zero address structure
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "./tin_socket");
	state = connect(socket, (struct sockaddr *) &address, sizeof(sockaddr_un));
	
	if(state == -1) {
		cout << "Socket file does not exist...\nIs server running?" << endl;
		exit(1);	
	}
	
	len = msg.length();

	write(socket, msg.c_str(), len);
	// TODO add timeout
	// TODO use something more meaningful than int.
	// TODO merge call for response here.
	read(socket, &result, 4);

	return "";
}
