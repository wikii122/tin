#ifndef CONNECTION_
#define CONNECTION_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

//! Klasa wyliczeniowa pokazująca, jakie stany może przyjąc połączenie
enum class ConnectionState
{
	Intro,
	PartInfo,
	Data,
	Finished,
};

//! Klasa reprezentująca jedno połączenie TCP
class Connection
{
	//! Metoda obsługująca połączenia przychodzące
	void handleIncoming();
	//! Metoda obsługująca połączenia wychodzące
	void handleOutgoing();
public:
	//! Konstruktor
	Connection();

	//! Adres
	sockaddr_in addr;

	//! Metoda obsługująca połączenie
	void handle();

	//! Bufor
	char* buffer;

	//! Dane pliku
	char* fileData;
	//! Zmienna informująca, jak dużo pliku zostało przesłane
	int transferred;

	//! Nazwa pliku
	std::string name;
	//! MD5 pliku
	std::string md5;
	//! Wielkość pliku
	long long size;
	//! Data ważności pliku
	long long expiry;

	//! Przesunięcie aktualnie wysyłanej/pobieranej części w pliku
	long long offset;
	//! Wielkość pliku
	long long partSize;

	//! Czy transferowana jest własność pliku
	bool original;

	//! Stan połączenia
	ConnectionState state;
	//! Gniazdo połączenia
	int socket;
	//! Czy połączenie jest przychodzące czy wychodzące
	bool incoming;
};

#endif