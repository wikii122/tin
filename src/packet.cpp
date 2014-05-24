#include "packet.h"


Packet::Packet(void)
{
}


Packet::~Packet(void)
{
}

const PacketType Packet::getType()
{
	return this->type;
}