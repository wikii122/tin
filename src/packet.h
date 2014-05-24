/*
*	Class representing an abstract class Packet upon which are the other Packet-type classes based.
*	Author: Damian Rakowski
*	Date created: 24-05-2014
*/

#ifndef TIN_PACKET
#define TIN_PACKET

#include <string>

enum class PacketType
{
	Hello,
};

class Packet
{
public:
	Packet(void);
	~Packet(void);

	virtual std::string getData() = 0;
	static Packet* getPacket(std::string data);
	const PacketType getType();

protected:
	PacketType type;
};

#endif