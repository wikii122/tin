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

/*!
 * Otwiera unix domain socket pod adresem wskazywanym przez client::SOCKET_PATH,
 * przypisuje go jako własne połączenie oraz rozpoczyna nasłuchiwanie.
 */
ClientHandler::ClientHandler(): 
	Handler() {
	struct sockaddr_un address;

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socket_fd == -1)
		throw std::string("ClientHandler::ClientHandler: Could not create socket");

	// Zero address struct, fill it with address.
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;	
	strcpy(address.sun_path, client::SOCKET_PATH);
	printf("%s\n", address.sun_path);
	if (bind(socket_fd, (struct sockaddr*) &address, sizeof(address)) == -1) {
		printf("ERRNO: %i\n", errno);
		throw std::string("ClientHandler::ClientHandler: Could not bind socket");
	}
	listen(socket_fd, 5);
}

/*!
 * Zamyka utworzony socket.
 */
ClientHandler::~ClientHandler()
{
	close(socket_fd);
}

/*!
 * Oczekuje na najbliższe połączenie, po jego nawiązaniu tworzy nowy
 * obiekt ClientHandler::Connection odpowiadający właśnie nawiązanemu
 * połączeniu.
 * \warning Blokuje podczas akceptowania połączenia.
 * \return Deskryptor socketu z połączeniem.
 */
auto ClientHandler::accept() -> Connection* 
{
	int result;

	result = ::accept(socket_fd, 0, 0);
	if (connection != nullptr)
		delete connection;
	connection = new Connection(result);
	return connection;
}

/*!
 * Przesyła do aktualnie nawiązanego połączenia podany string.
 *
 * \param msg Message to be written.
 * \return Wartość zwrócona przez ClientHandler::Connection::write 
 * */
int ClientHandler::write(string msg)
{
	return connection->write(msg.c_str(), msg.length());
}

/*!
 * Wczytuje z aktualnie nawiązanego połączenia string
 * aż do momentu dostania pełnego zestawu odpowiadających
 * sobie klamr { oraz }.
 * \return Odczytana wiadomość.
 */
string ClientHandler::read()
{
	string msg = connection->read();
	cout << msg << endl;
	return msg;
}

/*!
 * Funkcja osługująca jednokrotne zaakceptowanie połączenia,
 * wczytanie polecenia od programu klienckiego, następnie
 * przetworzenie polecenia oraz zwrócenie odpowiedzi.
 *
 * \warning Powinna działać w pętli
 * \warning Blokuje podczas akceptowania połączenia
 * \warning Blokuje podczas odczytu
 *
 */
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
		auto list = server.get_storage_info().file_info(req->file);
		if (list.size() == 0) {
			json_resp["msg"] = "File not in network";
			json_resp["display"] = true;
			response = writer.write(json_resp);
			write(response);
			return 0;
		}
		string md5 = list[list.size()-1].md5;
		if (!server.get_storage().on_drive(name, md5)) {
			json_resp["msg"] = "File not downloaded";
			json_resp["display"] = true;
			/*auto packet = make_shared<GiveMePacket>();
			packet->filename = req->name;
			packet->md5 = list[list.size()-1].md5;
			packet->original = false;
			server.network().addToQueue(packet);*/
		}
		else {
			bool state = server.get_storage().copy_file(file, name);
			if (state) {
				json_resp["msg"] = "OK";
				json_resp["display"] = false;
			} else {
				json_resp["msg"] = "File could not be copied";
				json_resp["display"] = true;
			}	
		}
	} else if (req->command == "LocalRemove") {
		string name = req->file;
		string md5 = "";
		bool owner;
		auto list = server.get_storage_info().file_info(name);
		for(File file: list) {
			if (file.local) {
				md5 = file.md5;
				owner = file.isOwner;
			}
		}
		bool state = server.get_storage().remove_file(name, md5);
		if (md5 != "" and state) {
			json_resp["msg"] = "File removed";
			json_resp["display"] = false;
			if (owner) {
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
		//std::cout << "TATATATATATATA:" << req->file << std::endl;
		auto list = server.get_storage_info().file_info(req->file);
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
		packet->filename = req->file;
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
			Storage_info::get().clear();
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

/*! 
 * Creates new connection with give socket descriptor.
 * \param fd connected socket descritor
 */
ClientHandler::Connection::Connection(int fd): 
	socket_fd(fd) 
{

}

/*!
 * Zamyka trzymane połączenie.
 */
ClientHandler::Connection::~Connection()
{
	close(socket_fd);
}

/*!
 * Przekazuje podane dane do połączenia.
 * \param msg Łańcuch znaków do przesłania
 * \param len długość msg
 * \return Ilość zapisanych znaków
 */
int ClientHandler::Connection::write(const char msg[], int len)
{
	return ::write(socket_fd, msg, len);
}

/*!
 * Wczytuje łańcuch z bufora aż do wystąpienia ostatniej odpowiadającej
 * sobie pary { i }.
 * \return Odczytany łańcuch znaków.
 */
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
