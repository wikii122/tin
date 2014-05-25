/*
 * Set of functions used to control the state 
 * of server.
 */
#include <string>
#ifndef RUN_H_
#define RUN_H_

namespace server {
	int start(std::string name);
	int stop();
	int restart(std::string name);
}

#endif
