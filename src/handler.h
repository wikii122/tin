/*
 * Extend this in order to implement inner socket handlers.
 */

#include <string>
#ifndef HANDLER_H_
#define HANDLER_H_

//! Abstrakcyjna klasa to zarzadzania socketami.
class Handler
{
public:
	Handler();
	Handler(const Handler&) = delete;
	virtual ~Handler();

	//! Funkcja wykonywana przez serwer.
	virtual int handle() = 0;
	//! Funkcja czyta wiadomość
	virtual auto read() -> std::string = 0;
	//! Funkcja wysyła wiadomość
	virtual int write(std::string) = 0;
protected:
	//! Deskryptor socketu do akceptowania połączeń.
	int socket_fd;
};

#endif
