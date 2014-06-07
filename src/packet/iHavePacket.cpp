#include <jsoncpp/json/json.h>
#include "packet/iHavePacket.h"
#include "server.h"


IHavePacket::IHavePacket(void)
{
	type = PacketType::IHave;
}


IHavePacket::~IHavePacket(void)
{
}

std::string IHavePacket::getData()
{
	if(name == "")
		name = Sever::get().get_name();
	Json::Value root;

	root["type"] = "IHave";
	root["name"] = name;

	Json::Value field;
	for(IHavePacketFile &file: files)
	{
		Json::Value f;
		f["file"] = file.name;
		f["md5"] = file.md5;
		f["expires"] = file.expires;
		f["isOwner"] = file.isOwner;
		

		field.append(f);
	}
	root["files"] = field;

	Json::StyledWriter writer;
	return  writer.write(root);
}
