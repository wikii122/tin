/*
*	Class representing a GiveMe-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_GIVEMEPACKET
#define TIN_GIVEMEPACKET

#include "packet/packet.h"
#include <vector>

class GiveMePacket :	public Packet
{
public:
	std::string name;
	std::string filename;
	std::string md5;
	bool original;

	std::string getData();
	GiveMePacket(void);
	~GiveMePacket(void);
};

#endif
