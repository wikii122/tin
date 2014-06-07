#include <jsoncpp/json/json.h>
#include "packet/localPacket.h"
#include "server.h"

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
		name = Server::get().get_name();
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
