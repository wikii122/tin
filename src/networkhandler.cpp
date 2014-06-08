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

NetworkHandler::NetworkHandler()
{
	createBroadcastSocket();
}

NetworkHandler::~NetworkHandler()
{
}

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

void NetworkHandler::addToQueue(std::shared_ptr<Packet> msg)
{
	queueMutex.lock();
	queue.push(msg);
	queueMutex.unlock();
}

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

	if (ret)
		return std::string(msg, size);
	else
		return "";
}

int NetworkHandler::write(std::string msg)
{
	if (sendto(sock, msg.c_str(), msg.length() + 1, 0, (sockaddr*) &addr, sizeof addr) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::write: Could not sendto");
	}
	return 0;
}

int NetworkHandler::respond(std::string msg)
{
	if (sendto(sock, msg.c_str(), msg.length() + 1, 0, (sockaddr*) &sender, sizeof sender) == -1) {
		printf("Error: %d\n", errno);
		throw std::string("NetworkHandler::respond: Could not sendto");
	}
	return 0;
}

void NetworkHandler::createBroadcastSocket()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		throw std::string("NetworkHandler::createBroadcastSocket: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = inet_addr("192.168.0.255");

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

	ifaddrs* addrs;
	getifaddrs(&addrs);
	
	while (addrs != NULL) {
		ownAddr.push_back(*((sockaddr_in*)addrs->ifa_addr));
		addrs = addrs->ifa_next;
	}

	freeifaddrs(addrs);
}

bool NetworkHandler::isObjected(std::string name, std::string md5)
{
	for(auto i : objected)
		if(i.first == name && i.second == md5)
			return true;

	return false;
}

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

std::vector<std::pair<std::string, std::string>> NetworkHandler::getFiles()
{
	return reportedFiles;
}

void NetworkHandler::clearFiles()
{
	reportedFiles.clear();
}

void NetworkHandler::handlePacket(std::shared_ptr<GiveFileListPacket> packet)
{
	 IHavePacket i_have_packet = Storage_info::get().list_files_json(false);
   		 respond(i_have_packet.getData());
}

void NetworkHandler::handlePacket(std::shared_ptr<IHavePacket> packet)
{
	for(auto x : packet->files)
	{
		reportedFiles.push_back(std::make_pair(x.name, x.md5));
	}
}

void NetworkHandler::handlePacket(std::shared_ptr<GiveMePacket> packet)
{
}

void NetworkHandler::handlePacket(std::shared_ptr<IGotPacket> packet)
{
}

void NetworkHandler::handlePacket(std::shared_ptr<ObjectionPacket> packet)
{
	objected.push_back(std::make_pair(packet->filename, packet->md5));
}

void NetworkHandler::handlePacket(std::shared_ptr<IForgotPacket> packet)
{
}

void NetworkHandler::handlePacket(std::shared_ptr<ForgetPacket> packet)
{
	
	Server::get().get_storage().remove_file(packet->filename, packet->md5);
	Server::get().get_storage_info().remove(packet->filename, packet->md5);
	
}
