#include "helloPacket.h"
#include "json/json.h"


HelloPacket::HelloPacket(void)
{
	name = "abc";
}


HelloPacket::~HelloPacket(void)
{
}

std::string HelloPacket::getData()
{
	Json::Value root;
	root["type"] = "Hello";
	root["name"] = name;
	Json::StyledWriter writer;
	return  writer.write(root);
}