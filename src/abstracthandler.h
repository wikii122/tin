/*
 * Extend this in order to implement inner socket handlers.
 */

#include <string>

// Feel free to change API
class AbstractHandler
{
	int socket_fd;
	void init();
public:
	AbstractHandler();
	AbstractHandler(const AbstractHandler&) = delete;
	~AbstractHandler();

	virtual int handle();
	virtual auto read() -> std::string = 0;
	virtual int write(char* msg, int len) = 0;
};
