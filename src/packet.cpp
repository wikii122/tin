#include "packet.h"

#include "helloPacket.h"
#include "giveFileListPacket.h"
#include "iHavePacket.h"

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
	else if(type == "GiveFileList")
	{
		GiveFileListPacket* giveFileListPacket = new GiveFileListPacket();
		giveFileListPacket->name = root.get("name", "").asString();

		packet = giveFileListPacket;
	}
	else if(type == "IHave")
	{
		IHavePacket* iHavePacket = new IHavePacket();
		iHavePacket-> name = root.get("name", "").asString();
		
		for(int i = 0; i < root.get("files", "").size(); ++i)
		{
			IHavePacketFile file;
			file.name = root.get("files", "")[i]["name"].asString();
			file.md5 = root.get("files", "")[i]["md5"].asString();
			file.expires = root.get("files", "")[i]["expires"].asInt64();
			file.isOwner = root.get("files", "")[i]["isOwner"].asBool();

			iHavePacket->files.push_back(file);
		}

		packet = iHavePacket;
	}
	else
	{
		throw std::exception("Packet::getPacket: No type specified");
	}

	return packet;
}