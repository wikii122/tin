/*
*	Class representing a IForgot-type Packet
*	Author: Damian Rakowski
*	Date created: 02-06-2014
*/
#ifndef TIN_IFORGOTPACKET
#define TIN_IFORGOTPACKET

#include "packet.h"
#include <vector>

class IForgotPacket :	public Packet
{
public:
	std::string name;
	std::string filename;
	std::string md5;

	std::string getData();
	IForgotPacket(void);
	~IForgotPacket(void);
};

#endif

