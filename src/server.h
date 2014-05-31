#include <string>
#include <thread>
#include <vector>
#include "handler.h"

static char lock_file_name[] = "server.lock";

class Server
{
	static const int no_threads = 10;

	bool running;
	std::string name;
	std::vector<Handler*> handlers;
	void start(Handler* handler);
	std::thread threads[no_threads];
public:
	Server();
	~Server();
	
	int  set_name(std::string new_name);
	auto get_name() -> std::string;
	void serve();
	void register_handler(Handler* handler);
};
