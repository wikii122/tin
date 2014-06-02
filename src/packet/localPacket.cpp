#include <jsoncpp/json/json.h>
#include "localPacket.h"


LocalPacket::LocalPacket()
{
	type = PacketType::Local;
}


LocalPacket::~LocalPacket()
{

}

auto LocalPacket::getData() -> std::string
{
	if (name == "")
		throw "LocalPacket::getData(): No name entry";
	if (file == "")
		throw "LocalPacket::getData(): No filename entry";
	if (command == "")
		throw "LocalPacket::getData(): No command name";

	Json::Value root;

	root["type"] = command;
	root["name"] = name;
	root["file"] = file;

	Json::StyledWriter writer;
	return  writer.write(root);
}
