/*
*	Class representing a Local-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_LOCALPACKET
#define TIN_LOCALPACKET

#include "packet.h"
#include <vector>

class LocalPacket: public Packet
{
public:
	std::string name;
	std::string file;
	std::string command;

	auto getData() -> std::string override;

	LocalPacket();
	~LocalPacket();
};

#endif

