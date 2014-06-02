#include <jsoncpp/json/json.h>
#include "iGotPacket.h"


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
		throw "IGotPacket::getData(): No name entry";
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
