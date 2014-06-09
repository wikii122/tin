/*
 * Module responsible for handling TCP networking
 */

#ifndef CONNECTION_HANDLER_
#define CONNECTION_HANDLER_
#include "handler.h"
#include "packet/packet.h"
#include "packet/giveFileListPacket.h"
#include "packet/iHavePacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "connection.h"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <queue>
#include <mutex>

//! Klasa zajmująca się obsługą TCP
class ConnectionHandler: public Handler
{
	int listener;

	//! Lista połączeń
	std::vector<Connection> connections;

	fd_set master;
	int maxsock;

	//! Metoda tworząca gniazdo TCP
	void createListenerSocket();
public:

	//! Konstruktor
	ConnectionHandler();
	//! Destruktor
	~ConnectionHandler();

	//! Żądanie wysłania pliku pod wskazany adres przy następnej sposobności
	void upload(std::string name, std::string md5, long long expiry, long long size, sockaddr_in addr, bool original);

	//! Metoda obsługująca TCP
	virtual int handle() override; //Blocking!
	//! Nie zaimplementowane
	virtual auto read() -> std::string override; // Blocking
	//! Nie zaimplementowane
	virtual int write(std::string) override;

	//! Mutex dający thread-safety
	std::mutex mutex;
};
#endif
