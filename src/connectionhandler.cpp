#include "connectionhandler.h"

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

#include "packet/packet.h"

//! Konstruktor
ConnectionHandler::ConnectionHandler()
{
	FD_ZERO(&master);
	createListenerSocket();
	FD_SET(listener, &master);
}

//! Destruktor
ConnectionHandler::~ConnectionHandler()
{
}

/*!
 * Metoda, która obsługuje istniejące połączenia TCP oraz akceptuje nowe. Powinna być często wywoływana
 */
int ConnectionHandler::handle()
{
	fd_set read;
	fd_set write;
	memcpy(&read, &master, sizeof(master));
	memcpy(&write, &master, sizeof(master));

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	if (select(maxsock+1, &read, &write, NULL, &timeout) != 0) {
		if(FD_ISSET(listener, &read)) {
			sockaddr_in addr;
			unsigned int addrLen = sizeof(addr);
			int sock = accept(listener, (sockaddr*)&addr, &addrLen);

			Connection conn;
			conn.incoming = true;
			conn.socket = sock;

			if(sock > maxsock)
				maxsock = sock;

			FD_SET(conn.socket, &master);

			mutex.lock();
				connections.push_back(conn);
			mutex.unlock();
		}

		mutex.lock();
		for(auto& conn : connections) {
			if (conn.incoming) {
				if(FD_ISSET(conn.socket, &write) && (conn.state == ConnectionState::PartInfo))
					conn.handle();
				if(FD_ISSET(conn.socket, &read) && (conn.state == ConnectionState::Intro || conn.state == ConnectionState::Data))
					conn.handle();
			}
			else {
				if(FD_ISSET(conn.socket, &read) && (conn.state == ConnectionState::PartInfo))
					conn.handle();
				if(FD_ISSET(conn.socket, &write) && (conn.state == ConnectionState::Intro || conn.state == ConnectionState::Data))
					conn.handle();
			}
		}
		mutex.unlock();
	}

	mutex.lock();
	for(auto& conn : connections) {
		if (conn.socket == -1) {
				if (-1 == (conn.socket = socket(AF_INET, SOCK_STREAM, 0))) {
					std::cout << "ERRNO " << errno << std::endl;
					throw std::string("ConnectionHandler::handle: Could not create socket");
				}
				if (-1 == connect(conn.socket, (sockaddr*)&(conn.addr), sizeof conn.addr))
					throw std::string("ConnectionHandler::handle: Could not connect");

				if(conn.socket > maxsock)
					maxsock = conn.socket;

				FD_SET(conn.socket, &master);
			}
	}
	mutex.unlock();

	return 0;
}

/*!
 * Żądanie wysłania pliku pod wskazany adres przy następnej sposobności. Plik zostanie wysłany przy następnym handle()
 * \param name Nazwa pliku
 * \param md5 MD5 pliku
 * \param expire Data ważności pliku
 * \param size Wielkość pliku
 * \param addr Adres celu
 * \param original Czy jest to transfer wraz z transferem własności pliku
 */
void ConnectionHandler::upload(std::string name, std::string md5, long long expire, long long size, sockaddr_in addr, bool original)
{
	Connection conn;
	conn.socket = -1;
	conn.incoming = false;
	conn.name = name;
	conn.md5 = md5;
	conn.expiry = expire;
	conn.size = size;
	conn.addr = addr;
	conn.original = original;

	mutex.lock();
		connections.push_back(conn);
	mutex.unlock();
}
 
/*!
 * Metoda niezaimplementowana
 */
auto ConnectionHandler::read() -> std::string
{
	return "";
}

/*!
 * Metoda niezaimplementowana
 */
int ConnectionHandler::write(std::string msg)
{
	return 0;
}

/*!
 * Metoda tworzące gniazdo TCP nasłuchujące na wszystkich interfejsach, porcie 31337
 */
void ConnectionHandler::createListenerSocket()
{
	sockaddr_in addr;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		throw std::string("ConnectionHandler::createListenerSocket: Could not create socket");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(31337);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listener, (sockaddr*)&addr, sizeof(addr)) == -1) {
		throw std::string("ConnectionHandler::createListenerSocket: Could not bind socket");
	}

	listen(listener, 5);

	maxsock = listener;
}