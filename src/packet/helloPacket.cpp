#include <jsoncpp/json/json.h>
#include "helloPacket.h"


HelloPacket::HelloPacket(void)
{
	type = PacketType::Hello;
}


HelloPacket::~HelloPacket(void)
{
}

std::string HelloPacket::getData()
{
	if(name == "")
		throw "HelloPacket::getData(): No name entry";

	Json::Value root;

	root["type"] = "Hello";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
