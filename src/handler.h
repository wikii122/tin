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
	Handler() = default;
	Handler(const Handler&) = delete;
	virtual ~Handler() = default;

	virtual int handle();
	virtual auto read() -> std::string = 0;
	virtual int write(char msg[], int len) = 0;
protected:
	int socket_fd;
};

#endif
