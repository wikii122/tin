#include <jsoncpp/json/json.h>
#include "packet/iGotPacket.h"
#include "server.h"

IGotPacket::IGotPacket(void)
{
	type = PacketType::IHave;
}


IGotPacket::~IGotPacket(void)
{
}

std::string IGotPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "IGotPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "IGot";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
