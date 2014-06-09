#include "networkhandler.h"

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <server.h>

#include "packet/packet.h"
#include "files/storage_info.h"

//! Konstruktor
NetworkHandler::NetworkHandler()
{
	createBroadcastSocket();
}

//! Destruktor
NetworkHandler::~NetworkHandler()
{
}
/*!
 * Metoda, która powinna być wywoływana często. Obsługuje ona ruch UDP
 */
int NetworkHandler::handle()
{
	queueMutex.lock();
		while(!queue.empty()) {
			std::shared_ptr<Packet> msg = queue.front();
			queueMutex.unlock();
			write(msg->getData());
			queueMutex.lock();
			queue.pop();
		}
	queueMutex.unlock();

	buffer += read();
	std::string packetString;

	while(buffer.size() != 0) {
		int brackets = 0; //for packet string analysis
		bool isPacket = false;
		unsigned int i;
		for (i = 0; (i < buffer.length()) && (!isPacket || brackets != 0); i++) {
			packetString += buffer[i];

			if (buffer[i] == '{') {
				isPacket = true;
				brackets++;
			}
			else if (buffer[i] == '}') {
				brackets--;
			}
			if (!isPacket) {
				buffer.erase(0, i+1);
				i = 0;
			}
		}

		if(!isPacket || brackets != 0) {
			break;
		}

		buffer.erase(0, i);

		std::shared_ptr<Packet> packet = Packet::getPacket(packetString);

		packetString.clear();

		switch(packet->getType())
		{
		case PacketType::GiveFileList:
			handlePacket(std::static_pointer_cast<GiveFileListPacket>(packet));
			break;
		case PacketType::IHave:
			handlePacket(std::static_pointer_cast<IHavePacket>(packet));
			break;
		case PacketType::GiveMe:
			handlePacket(std::static_pointer_cast<GiveMePacket>(packet));
			break;
		case PacketType::IGot:
			handlePacket(std::static_pointer_cast<IGotPacket>(packet));
			break;
		case PacketType::Objection:
			handlePacket(std::static_pointer_cast<ObjectionPacket>(packet));
			break;
		case PacketType::IForgot:
			handlePacket(std::static_pointer_cast<IForgotPacket>(packet));
			break;
		case PacketType::Forget:
			handlePacket(std::static_pointer_cast<ForgetPacket>(packet));
			break;
		default:
			break;
		}
	}
	return 0;
}

/*!
 * Metoda, która dodaje pakiet do kolejki oczekujących na wysłanie. Pakiet zostanie wysłany w następnym handle().
 * \param msg Wiadomość, jaka ma został wysłana
 * \warning Jest to thread-safe metoda
 */
void NetworkHandler::addToQueue(std::shared_ptr<Packet> msg)
{
	queueMutex.lock();
	queue.push(msg);
	queueMutex.unlock();
}

/*!
 * Metoda, która odczytuje wiadomość wysłaną do tego komputera za pomocą UDP i ją zwraca. Jest nieblokująca.
 * \return Odczytana wiadomość
 */
auto NetworkHandler::read() -> std::string
{
	char msg[1024];
	msg[0] = 0;

	sendersize = sizeof sender;
	memset(&sender, 0, sizeof sender);

	int size;

	if ((size = recvfrom(sock, msg, 1024, 0, (sockaddr*) &sender, &sendersize)) == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::read: Could not recvfrom");
	}

	bool ret = false;
	if(size >= 0) {
		ret = true;
		for(auto a = ownAddr.begin(); a != ownAddr.end(); ++a)
			if (a->sin_addr.s_addr == sender.sin_addr.s_addr)
				ret = false;
	}

	if (ret) {
		std:: cout << "Received:" << std::endl << std::string(msg, size) << std::endl;
		return std::string(msg, size);
	}
	else
		return "";
}

/*!
 * Metoda broadcastująca wiadomość.
 * \param msg Wiadomość do zbroadcastowania.
 */
int NetworkHandler::write(std::string msg)
{
	if (sendto(sock, msg.c_str(), msg.length() + 1, 0, (sockaddr*) &addr, sizeof addr) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::write: Could not sendto");
	}
	std:: cout << "Broadcasted:" << std::endl << msg << std::endl;
	return 0;
}

/*!
 * Metoda odpowiadająca komputerowi, od którego otrzymana ostatnią wiadomość.
 * \param msg Wiadomość do wysłania
 */
int NetworkHandler::respond(std::string msg)
{
	if (sendto(sock, msg.c_str(), msg.length() + 1, 0, (sockaddr*) &sender, sizeof sender) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::respond: Could not sendto");
	}
	std:: cout << "Directly sent:" << std::endl << msg << std::endl;
	return 0;
}

/*!
 * Metoda tworząca gniazdo.
 * \warning Jest należy przechwytywać wyjątek.
 */
void NetworkHandler::createBroadcastSocket()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);

	addr.sin_addr.s_addr = INADDR_ANY;

	const int isTrue = 1;
	timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &isTrue, sizeof(isTrue)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not setsockopt(BROADCAST)");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &isTrue, sizeof(isTrue)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not setsockopt(REUSEADDR)");
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not setsockopt(SRCVTIMEO)");

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
		throw std::string("NetworkHandler::createBroadcastSocket: Could not bind socket");
	}

	addr.sin_addr.s_addr = inet_addr("192.168.1.255");

	ifaddrs* addrs;
	getifaddrs(&addrs);
	
	while (addrs != NULL) {
		ownAddr.push_back(*((sockaddr_in*)addrs->ifa_addr));
		addrs = addrs->ifa_next;
	}

	freeifaddrs(addrs);
}

/*!
 * Metoda informująca, czy ktoś sprzeciwił się danemu plikowi.
 * \param name Nazwa pliku
 * \param md5 MD5 pliku
 * \return True jeżeli ktoś się sprzeciwił, w innym wypadku false;
 */
bool NetworkHandler::isObjected(std::string name, std::string md5)
{
	for(auto i : objected)
		if(i.first == name && i.second == md5)
			return true;

	return false;
}

/*!
 * Metoda czyszcząca sprzeciwy wobec danej nazwy i danego md5 naraz
 */
void NetworkHandler::clearObjected(std::string name, std::string md5)
{
	auto i = 0;
	for(auto it : objected)
	{
		if(it.first != name || it.second != md5)
			objected[i++] = it;
	}
	objected.resize(i);
}

/*!
 * Metoda, która zwraca pliki istniejące w sieci na podstawie uzyskanych pakietów IHave
 * \return Wektor par (nazwa pliku, md5 pliku)
 */
std::vector<std::pair<std::string, std::string>> NetworkHandler::getFiles()
{
	return reportedFiles;
}

/*!
 * Metoda, która czyści listę plików znanych w sieci
 */
void NetworkHandler::clearFiles()
{
	reportedFiles.clear();
}

/*!
 * Metoda, która odpowiada na pakiet GiveFileList pakietem IHave z odpowiednią listą plików posiadanych lokalnie.
 * \param packet Pakiet typu GiveFileList
 */
void NetworkHandler::handlePacket(std::shared_ptr<GiveFileListPacket> packet)
{
	 IHavePacket i_have_packet = Storage_info::get().list_files_json(false);
	 respond(i_have_packet.getData());
}

/*!
 * Metoda, która zapamiętuje pliki zgłoszone przez pakiet typu IHave
 * \param packet Pakiet typu IHave
 */
void NetworkHandler::handlePacket(std::shared_ptr<IHavePacket> packet)
{
	for(auto x : packet->files)
	{
		Storage_info::get().add_file(x.name, false, x.expires, x.md5, false); 
		//reportedFiles.push_back(std::make_pair(x.name, x.md5));
	}
}

/*!
 * Metoda, która inicjuje transfer po otrzymaniu żądania typu GiveMe, o ile żądanie jest poprawne
 * \param packet Pakiet typu GiveMe
 */
void NetworkHandler::handlePacket(std::shared_ptr<GiveMePacket> packet)
{
	std::vector<File> files = Storage_info::get().file_info(packet->filename);
	for (auto file : files)
		if (file.md5 == packet->md5) {
			if(!(file.isOwner == false && packet->original == true)) {
				std::shared_ptr<LoadedFile> lfile = Server::get().get_storage().get_file(packet->filename, packet->md5);
				Server::get().connection().upload(file.name, file.md5, file.expire_date, lfile->size, sender, packet->original);
			}
			if(file.isOwner && packet->original == true) {
				std::cout << "Losing ownership of " << file.name << std::endl;
				Storage_info::get().set_ownership(file.name, file.md5, false);
			}
		}
}

/*!
 * Metoda, która zastanawia się nad zezwoleniem na istnienie pliku oraz ewentualnym przejęciem własności nad nim i odpowiedziem w wypadku konieczności zgłoszenia sprzeciwu, a także wysłaniem pakietu typu GiveMe jeżeli istnieje powinność przejęcia pliku.
 * \param packet Pakiet typu IGot
 */
void NetworkHandler::handlePacket(std::shared_ptr<IGotPacket> packet)
{
	std::vector<File> files = Storage_info::get().file_info(packet->filename);
	for (auto file : files) {
		if (file.md5 == packet->md5) {
			ObjectionPacket response;
			response.filename = packet->filename;
			response.md5 = packet->md5;
			respond(response.getData());
			return;
		}
	}
	auto resp = std::make_shared<GiveMePacket>();

	resp->filename = packet->filename;
	resp->md5 = packet->md5;
	resp->original = true;

	respond(resp->getData());
}

/*!
 * Metoda, która akceptuje sprzeciw wobec wcześniejszego pakietu typu IGot, pamiętając, że nie jest się ownerem
 * \param packet Pakiet typu Objection
 */
void NetworkHandler::handlePacket(std::shared_ptr<ObjectionPacket> packet)
{
	// Assuming file is not on drive yet
	//Storage_info::get().remove(packet->name, packet->md5);
	auto list = Storage_info::get().file_info(packet->name);
	for (File f: list) {
		if (f.md5 == packet->md5) {
			Storage_info::get().add_file(f.name, false, f.expire_date, f.md5, f.local);
			break;
		}
	}
	//objected.push_back(std::make_pair(packet->filename, packet->md5));
}

/*!
 * Metoda, która może obsłużć pakiet typu IForgot. Aktualnie ten typ pakietów jest ignorowany
 * \param packet Pakiet typu IForgot
 */
void NetworkHandler::handlePacket(std::shared_ptr<IForgotPacket> packet)
{
}

/*!
 * Metoda, która akceptuje wymaganie usunięcia pliku.
 * \param packet Pakiet typu Forget
 */
void NetworkHandler::handlePacket(std::shared_ptr<ForgetPacket> packet)
{
	
	Server::get().get_storage().remove_file(packet->filename, packet->md5);
	Server::get().get_storage_info().remove(packet->filename, packet->md5);
	
}
