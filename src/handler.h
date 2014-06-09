/*
 * Extend this in order to implement inner socket handlers.
 */

#include <string>
#ifndef HANDLER_H_
#define HANDLER_H_

//! Abstract class to handle sockets
class Handler
{
public:
	Handler();
	Handler(const Handler&) = delete;
	virtual ~Handler();

	//! Function invoked by server
	virtual int handle() = 0;
	//! Function to read message
	virtual auto read() -> std::string = 0;
	//! Function to write message
	virtual int write(std::string) = 0;
protected:
	//! Socket descriptor for accepting
	int socket_fd;
};

#endif
