#include <jsoncpp/json/json.h>
#include "packet/giveFileListPacket.h"
#include "server.h"

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
		name = Server::get().get_name();

	Json::Value root;

	root["type"] = "GiveFileList";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
