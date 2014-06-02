#include <jsoncpp/json/json.h>
#include "forgetPacket.h"


ForgetPacket::ForgetPacket(void)
{
	type = PacketType::IHave;
}


ForgetPacket::~ForgetPacket(void)
{
}

std::string ForgetPacket::getData()
{
	if(name == "")
		throw "ForgetPacket::getData(): No name entry";
	if(filename == "")
		throw "ForgetPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "Forget";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
