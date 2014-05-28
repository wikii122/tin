/*
 * Client's network interface.
 */
#include <string>
#ifndef NET_H_
#define NET_H_

namespace client {
		int download(std::string filename, std::string nodename);
		int download(std::string filename);
		int rescan();
		int showlist();
}

#endif
