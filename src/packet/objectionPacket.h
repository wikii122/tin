/*
*	Class representing a Objection-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_OBJECTIONPACKET
#define TIN_OBJECTIONPACKET

#include "packet.h"
#include <vector>

class ObjectionPacket :	public Packet
{
public:
	std::string name;
	std::string filename;
	std::string md5;

	std::string getData();
	ObjectionPacket(void);
	~ObjectionPacket(void);
};

#endif

