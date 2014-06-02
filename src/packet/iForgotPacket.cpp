#include <jsoncpp/json/json.h>
#include "iForgotPacket.h"


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
		throw "IForgotPacket::getData(): No name entry";
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
