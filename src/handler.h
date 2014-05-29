/*
 * Extend this in order to implement inner socket handlers.
 */

#include <string>

// Feel free to change API
class Handler
{
public:
	Handler() = default;
	Handler(const Handler&) = delete;
	~Handler() = default;

	virtual int handle();
	virtual auto read() -> std::string = 0;
	virtual int write(char* msg, int len) = 0;
protected:
	int socket_fd;
};
