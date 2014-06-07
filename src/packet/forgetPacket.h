/*
*	Class representing a Forget-type Packet
*	Author: Damian Rakowski
*	Date created: 02-06-2014
*/
#ifndef TIN_FORGETPACKET
#define TIN_FORGETPACKET

#include "packet/packet.h"
#include <vector>


class ForgetPacket :	public Packet
{
public:
	std::string name;
	std::string filename;
	std::string md5;

	std::string getData();
	ForgetPacket(void);
	~ForgetPacket(void);
};

#endif

