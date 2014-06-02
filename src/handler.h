/*
 * Extend this in order to implement inner socket handlers.
 */

#include <string>
#ifndef HANDLER_H_
#define HANDLER_H_

// Feel free to change API
class Handler
{
public:
	Handler();
	Handler(const Handler&) = delete;
	virtual ~Handler();

	virtual int handle() = 0;
	virtual auto read() -> std::string = 0;
	virtual int write(std::string) = 0;
protected:
	int socket_fd;
};

#endif
