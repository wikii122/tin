#include <jsoncpp/json/json.h>
#include "giveFileListPacket.h"

GiveFileListPacket::GiveFileListPacket(void)
{
	type = PacketType::GiveFileList;
}


GiveFileListPacket::~GiveFileListPacket(void)
{
}

std::string GiveFileListPacket::getData()
{
	if(name == "")
		throw "GiveFileListPacket::getData(): No name entry";

	Json::Value root;

	root["type"] = "GiveFileList";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
