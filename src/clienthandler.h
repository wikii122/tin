/*
 * Module responsible for reading input from console client.
 * Opens it's own socket, with hardcoded file address, and
 * keeps it open for life of the process.
 */

#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_
#include <string>
#include <sys/socket.h>
#include "handler.h"

/*!
 * \brief Moduł odpowiedzialny za komunikację z klientem
 *
 * To jest moduł serwera rozszerzający klasę Handler o możliwości
 * współpracy z programem klienckim. Jest odpowiedzialny za
 * parsowanie i odpowiadanie na pakiety oraz całość komunikacji
 * z użytkonikiem.
 */
class ClientHandler: public Handler
{
	 //! \brief Reprezentacja pojedyńczego połączenia.
	class Connection;
	//! \brief Aktualne połączenie
	Connection* connection = nullptr;
	//! \brief Oczekiwanie na nowe połączenie.
	Connection* accept();
public:
	//! \brief Tworzy obiekt klasy, przygowując niezbędne połączenia.
	ClientHandler();
	//! \brief Destruktor do obiektu.
	~ClientHandler();

	//! \brief Wykonanie cyklu obsługi klienta 
	virtual int handle() override; //Blocking!
	//! \brief Wczytuje z aktualnego połączenia jeden pakiet JSONa.
	virtual auto read() -> std::string override; // Blocking
	//! \brief Wysyła podanego stringa go klienta
	virtual int write(std::string) override;
};

/*!
 * Instacja zawiera jedno zawarte z klientem połączenie.
 */
class ClientHandler::Connection
{
	int socket_fd;
public:
	Connection(int fd);
	~Connection();
	//! \brief Odczytuje JSON
	auto read() -> std::string;
	//! \brief Wysyła podany JSON
	int write(const char msg[], int len);
};

#endif
