#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "clienthandler.h"
#include "server.h"
#include "client/client.h"
#include "files/storage.h"
#include "files/storage_info.h"
#include "packet/localPacket.h"
#include "packet/packet.h"

using namespace std;

ClientHandler::ClientHandler(): 
	Handler()
{
	struct sockaddr_un address;

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);		

	// Zero address struct, fill it with address.
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;	
	strcpy(address.sun_path, client::SOCKET_PATH);
	bind(socket_fd, (struct sockaddr*) &address, sizeof(address));
	listen(socket_fd, 5);
}

ClientHandler::~ClientHandler()
{
	close(socket_fd);
}

auto ClientHandler::accept() -> Connection* 
{
	int result;

	result = ::accept(socket_fd, 0, 0);
	if (connection != nullptr)
		delete connection;
	connection = new Connection(result);
	return connection;
}

int ClientHandler::write(string msg)
{
	return connection->write(msg.c_str(), msg.length());
}

string ClientHandler::read()
{
	string msg = connection->read();
	cout << msg << endl;
	return msg;
}

int ClientHandler::handle()
{
	accept();
	// Now new connection needs to be handled.
	string msg = read();
	string response;
	auto req = static_pointer_cast<LocalPacket>(Packet::getPacket(msg));
	// TODO do this fancier (function overloading)
	// TODO response handling...
	if (req->command == "LocalFileAdd") {
		string name = req->name;
		string file = req->file;
		bool state = Server::get().get_storage().add_file(file, name);
		if (state) {
			// TODO write response
		} else {
			// TODO write negative response
		}	
	} else if (req->command == "LocalFileGet") {
		string name = req->name;
		string file = req->file;
		bool state = Server::get().get_storage().copy_file(file, name);
		if (state) {
			// TODO write response
		} else {
			// TODO write negative response
		}	
	} else if (req->command == "LocalRemove") {
		string name = req->name;
		bool state = Server::get().get_storage().remove_file(name);
		if (state) {
			// TODO write response
		} else {
			// TODO write negative response
		}	
	} else if (req->command == "LocalDownload") {

	} else if (req->command == "LocalRequest") {
		string response = Server::get().get_storage_info().list_files_json();
	} else {
		throw string("Packet::getPacket(): Unknown command");
	}
	write(response);

	return 0;
}

ClientHandler::Connection::Connection(int fd): 
	socket_fd(fd) 
{

}

ClientHandler::Connection::~Connection()
{
	close(socket_fd);
}

int ClientHandler::Connection::write(const char msg[], int len)
{
	return ::write(socket_fd, msg, len);
}

string ClientHandler::Connection::read()
{
	// This requires thing to parse JSON input.
	char sign = 0; // Assigning to get rid of uninitialization warning.
	char buffer[111];
	int i = 0;
	while (sign != '}') {
		::read(socket_fd, &sign, 1);
		buffer[i++] = sign;
	}
	return string(buffer, buffer+i);
}
