#include <jsoncpp/json/json.h>
#include "packet/giveMePacket.h"


GiveMePacket::GiveMePacket(void)
{
	type = PacketType::IHave;
}


GiveMePacket::~GiveMePacket(void)
{
}

std::string GiveMePacket::getData()
{
	if(name == "")
		throw "GiveMePacket::getData(): No name entry";
	if(filename == "")
		throw "GiveMePacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "GiveMe";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;
	root["original"] = original;

	Json::StyledWriter writer;
	return  writer.write(root);
}
