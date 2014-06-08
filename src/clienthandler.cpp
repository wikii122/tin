#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "clienthandler.h"
#include "server.h"
#include "client/client.h"
#include "files/storage.h"
#include "files/storage_info.h"
#include "packet/forgetPacket.h"
#include "packet/giveFileListPacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "packet/localPacket.h"
#include "packet/packet.h"

using namespace std;

ClientHandler::ClientHandler(): 
	Handler() {
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
	Json::StyledWriter writer;
	Json::Value json_resp;
	auto req = static_pointer_cast<LocalPacket>(Packet::getPacket(msg));
	Server& server = Server::get();
	if (req->command == "LocalFileAdd") {
		string state = server.get_storage().add_file(req->file, req->name);
		if (state != "") {
			// TODO expiry (should be zero or date? It's not owner after all)
			auto packet = make_shared<IGotPacket>();
			packet->filename = req->name;
			packet->md5 = state;
			server.network().addToQueue(packet);
				
			json_resp["msg"] = "OK";
			json_resp["display"] = false;
		} else {
			json_resp["msg"] = "File could not be added";
			json_resp["display"] = true;
		}	
	} else if (req->command == "LocalFileGet") {
		string name = req->name;
		string file = req->file;
		auto list = server.get_storage_info().file_info(req->name);
		if (list.size() == 0) {
			json_resp["msg"] = "File not in network";
			json_resp["display"] = true;
			response = writer.write(json_resp);
			write(response);
			return 0;
		}
		string md5 = list[list.size()-1].md5;
		if (!server.get_storage().on_drive(name, md5)) {
			auto packet = make_shared<GiveMePacket>();
			packet->filename = req->name;
			packet->md5 = list[list.size()-1].md5;
			packet->original = false;
			server.network().addToQueue(packet);
		}
		bool state = server.get_storage().copy_file(file, name);
		if (state) {
			json_resp["msg"] = "OK";
			json_resp["display"] = false;
		} else {
			json_resp["msg"] = "File could not be copied";
			json_resp["display"] = true;
		}	
	} else if (req->command == "LocalRemove") {
		string name = req->name;
		auto list = server.get_storage_info().file_info(name);
		string md5 = list[0].md5;
		bool state = server.get_storage().remove_file(name, md5);
		if (state) {
			json_resp["msg"] = "File removed";
			json_resp["display"] = false;
			if (list[0].isOwner) {
				auto packet = make_shared<ForgetPacket>();
				packet->filename = name;
				packet->md5 = md5;
				server.network().addToQueue(packet);
			}
		} else {
			json_resp["msg"] = "Failed to remove file";
			json_resp["display"] = true;
		}
	} else if (req->command == "LocalDownload") {
		auto packet = make_shared<GiveMePacket>();
		packet->filename = req->name;
		auto list = server.get_storage_info().file_info(req->name);
		if (list.size() == 0) {
			json_resp["msg"] = "File not in network";
			json_resp["display"] = true;
			response = writer.write(json_resp);
			write(response);
			return 0;
		}
		json_resp["msg"] = "Download pending";
		json_resp["display"] = true;
		packet->md5 = list[list.size()-1].md5;
		packet->original = false;
		server.network().addToQueue(packet);
	} else if (req->command == "LocalRequest") {
		if (req->name == "filelist") {
			Json::Reader reader;
			Json::Value files;
			json_resp["msg"] = "";
			json_resp["display"] = false;
			// FIXME This needs to be rewritten
			response = server.get_storage_info().list_files_json(true).getData();
			reader.parse(response, files);
			json_resp["files"] = files["files"];
		} else if (req->name == "rescan") {
			auto packet = make_shared<GiveFileListPacket>();
			server.network().addToQueue(packet);	
			json_resp["msg"] = "Refresh pending";
			json_resp["display"] = true;
		} else {
			json_resp["msg"] = "Unknown command";
			json_resp["display"] = true;
		}
	} else {
		throw string("ClientHandler::handle: Unknown command");
	}

	response = writer.write(json_resp);
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
	char buffer[1025];
	int i = 0;
	while (sign != '}') {
		::read(socket_fd, &sign, 1);
		buffer[i++] = sign;
	}
	return string(buffer, buffer+i);
}
