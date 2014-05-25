/*
 * Set of utilities used to communicate with server.
 */
#include <string>
#ifndef COMMAND_H_
#define COMMAND_H_

namespace communicate
{
	int call(std::string msg);
	auto callForResponse(std::string msg) -> std::string;
}

#endif
