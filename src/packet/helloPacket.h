/*
*	Class representing a Hello-type Packet
*	Author: Damian Rakowski
*	Date created: 24-05-2014
*/

#ifndef TIN_HELLOPACKET
#define TIN_HELLOPACKET

#include "packet/packet.h"

class HelloPacket : public Packet
{
public:
	std::string name;

	std::string getData();
	HelloPacket(void);
	~HelloPacket(void);
};

#endif
