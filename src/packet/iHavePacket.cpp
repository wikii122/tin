#include <jsoncpp/json/json.h>
#include "packet/iHavePacket.h"
#include "server.h"

//! Konstruktor
IHavePacket::IHavePacket(void)
{
	type = PacketType::IHave;
}

//! Destruktor
IHavePacket::~IHavePacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 */
std::string IHavePacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
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
