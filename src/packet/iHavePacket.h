/*
*	Class representing a IHave-type Packet
*	Author: Damian Rakowski
*	Date created: 31-05-2014
*/
#ifndef TIN_IHAVEPACKET
#define TIN_IHAVEPACKET

#include "packet.h"
#include <vector>

struct IHavePacketFile
{
	std::string name;
	std::string md5;
	bool isOwner;
	long long expires;
};

class IHavePacket :	public Packet
{
public:
	std::string name;
	std::vector<IHavePacketFile> files;

	std::string getData();
	IHavePacket(void);
	~IHavePacket(void);
};

#endif

