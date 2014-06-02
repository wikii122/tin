#include <jsoncpp/json/json.h>
#include "objectionPacket.h"


ObjectionPacket::ObjectionPacket(void)
{
	type = PacketType::IHave;
}


ObjectionPacket::~ObjectionPacket(void)
{
}

std::string ObjectionPacket::getData()
{
	if(name == "")
		throw "ObjectionPacket::getData(): No name entry";
	if(filename == "")
		throw "ObjectionPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "Objection";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
