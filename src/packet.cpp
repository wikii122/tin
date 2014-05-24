#include "packet.h"

#include "helloPacket.h"

#include "json/json.h"

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

Packet* Packet::getPacket(std::string data)
{
	Json::Value root; //the JSON tree will be stored here
	Json::Reader reader;

	Packet* packet;

	if(!reader.parse(data, root))
		throw std::exception("Packet::getPacket: Unparseable data");

	std::string type = root.get("type", "").asString();

	if(type == "Hello")
	{
		HelloPacket* helloPacket = new HelloPacket();
		helloPacket->name = root.get("name", "").asString();

		packet = helloPacket;
	}
	else
	{
		throw std::exception("Packet::getPacket: No type specified");
	}

	return packet;
}