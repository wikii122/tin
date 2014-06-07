#include <jsoncpp/json/json.h>
#include "packet/helloPacket.h"
#include "server.h"


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
		name = Sever::get().get_name();

	Json::Value root;

	root["type"] = "Hello";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
