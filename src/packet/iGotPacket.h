/*
*	Class representing a IGot-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_IGOTPACKET
#define TIN_IGOTPACKET

#include "packet/packet.h"
#include <vector>

class IGotPacket :	public Packet
{
public:
	std::string name;
	std::string filename;
	std::string md5;

	std::string getData();
	IGotPacket(void);
	~IGotPacket(void);
};

#endif

