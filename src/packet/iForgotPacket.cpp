#include <jsoncpp/json/json.h>
#include "packet/iForgotPacket.h"
#include "server.h"


IForgotPacket::IForgotPacket(void)
{
	type = PacketType::IHave;
}


IForgotPacket::~IForgotPacket(void)
{
}

std::string IForgotPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "IForgotPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "IForgot";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
