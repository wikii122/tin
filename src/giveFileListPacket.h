/*
*	Class representing a GiveFileList-type Packet
*	Author: Damian Rakowski
*	Date created: 31-05-2014
*/

#ifndef TIN_GIVEFILELISTPACKET
#define TIN_GIVEFILELISTPACKET

#include "packet.h"
class GiveFileListPacket : public Packet
{
public:
	std::string name;

	std::string getData();
	GiveFileListPacket(void);
	~GiveFileListPacket(void);
};

#endif